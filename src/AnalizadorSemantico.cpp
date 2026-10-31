#include "AnalizadorSemantico.hpp"

#include <QStringList>

namespace {

struct Token {
    QString original;
    QString normal;
};

QString normalizarCaracter(QChar c) {
    QString resultado(c);
    resultado = resultado.toLower();
    const QString conAcento = QStringLiteral("áéíóúüñ");
    const QString sinAcento  = QStringLiteral("aeiouun");
    for (int i = 0; i < conAcento.size(); ++i) {
        resultado.replace(conAcento[i], sinAcento[i]);
    }
    return resultado;
}

QVector<Token> tokenizar(const QString& linea) {
    QVector<Token> tokens;
    QString actualOriginal;
    QString actualNormal;

    auto volcar = [&]() {
        if (!actualOriginal.isEmpty()) {
            tokens.append(Token{actualOriginal, actualNormal});
            actualOriginal.clear();
            actualNormal.clear();
        }
    };

    for (const QChar c : linea) {
        if (c == QLatin1Char('(') || c == QLatin1Char(')')) {
            volcar();
            tokens.append(Token{QString(c), QString(c)});
        } else if (c.isSpace() || c == QLatin1Char(',') || c == QLatin1Char('.')) {
            volcar();
        } else {
            actualOriginal.append(c);
            actualNormal.append(normalizarCaracter(c));
        }
    }
    volcar();
    return tokens;
}

bool esOperador(const QString& normal) {
    return normal == QStringLiteral("y")
        || normal == QStringLiteral("o")
        || normal == QStringLiteral("no")
        || normal == QStringLiteral("si")
        || normal == QStringLiteral("entonces");
}

class ParserInterno {
public:
    explicit ParserInterno(const QVector<Token>& tokens) : m_tokens(tokens) {}

    FormulaPtr parse(QString& error) {
        FormulaPtr formula = parseBicondicional();
        if (!m_fallo && m_pos < m_tokens.size()) {
            errorInterno(QStringLiteral("Token inesperado: '") + m_tokens[m_pos].original + QStringLiteral("'"));
        }
        if (m_fallo) {
            error = m_mensaje;
            return nullptr;
        }
        return formula;
    }

private:
    QVector<Token> m_tokens;
    int m_pos = 0;
    bool m_fallo = false;
    QString m_mensaje;

    void errorInterno(const QString& mensaje) {
        if (!m_fallo) { m_fallo = true; m_mensaje = mensaje; }
    }
    bool fin() const { return m_pos >= m_tokens.size(); }
    QString actualNormal() const { return fin() ? QString() : m_tokens[m_pos].normal; }
    QString actualOriginal() const { return fin() ? QString() : m_tokens[m_pos].original; }
    bool es(const QString& normal) const { return actualNormal() == normal; }

    bool esBicondicional() const {
        return m_pos + 3 < m_tokens.size()
            && m_tokens[m_pos].normal     == QStringLiteral("si")
            && m_tokens[m_pos + 1].normal == QStringLiteral("y")
            && m_tokens[m_pos + 2].normal == QStringLiteral("solo")
            && m_tokens[m_pos + 3].normal == QStringLiteral("si");
    }

    FormulaPtr parseBicondicional() {
        FormulaPtr izquierda = parseImplicacion();
        if (m_fallo) return nullptr;
        while (esBicondicional()) {
            m_pos += 4;
            FormulaPtr derecha = parseImplicacion();
            if (m_fallo) return nullptr;
            izquierda = Formula::crearBicondicional(izquierda, derecha);
        }
        return izquierda;
    }

    FormulaPtr parseImplicacion() {
        if (es(QStringLiteral("si")) && !esBicondicional()) {
            ++m_pos;
            FormulaPtr condicion = parseBicondicional();
            if (m_fallo) return nullptr;
            if (!es(QStringLiteral("entonces"))) {
                errorInterno(QStringLiteral("Falta 'entonces' después de 'si'"));
                return nullptr;
            }
            ++m_pos;
            FormulaPtr cuerpo = parseImplicacion();
            if (m_fallo) return nullptr;
            return Formula::crearImplicacion(condicion, cuerpo);
        }
        return parseDisyuncion();
    }

    FormulaPtr parseDisyuncion() {
        FormulaPtr izquierda = parseConjuncion();
        if (m_fallo) return nullptr;
        while (es(QStringLiteral("o"))) {
            ++m_pos;
            FormulaPtr derecha = parseConjuncion();
            if (m_fallo) return nullptr;
            izquierda = Formula::crearDisyuncion(izquierda, derecha);
        }
        return izquierda;
    }

    FormulaPtr parseConjuncion() {
        FormulaPtr izquierda = parseUnario();
        if (m_fallo) return nullptr;
        while (es(QStringLiteral("y"))) {
            ++m_pos;
            FormulaPtr derecha = parseUnario();
            if (m_fallo) return nullptr;
            izquierda = Formula::crearConjuncion(izquierda, derecha);
        }
        return izquierda;
    }

    FormulaPtr parseUnario() {
        if (es(QStringLiteral("no"))) {
            ++m_pos;
            FormulaPtr interior = parseUnario();
            if (m_fallo) return nullptr;
            return Formula::crearNegacion(interior);
        }
        return parsePrimario();
    }

    FormulaPtr parsePrimario() {
        if (fin()) {
            errorInterno(QStringLiteral("Se esperaba una proposición"));
            return nullptr;
        }
        if (es(QStringLiteral("("))) {
            ++m_pos;
            FormulaPtr interior = parseBicondicional();
            if (m_fallo) return nullptr;
            if (!es(QStringLiteral(")"))) {
                errorInterno(QStringLiteral("Paréntesis desbalanceados: falta ')'"));
                return nullptr;
            }
            ++m_pos;
            return interior;
        }
        if (es(QStringLiteral(")"))) {
            errorInterno(QStringLiteral("Paréntesis desbalanceados: ')' inesperado"));
            return nullptr;
        }
        if (esOperador(actualNormal())) {
            errorInterno(QStringLiteral("Se esperaba una proposición, pero se encontró '") + actualOriginal() + QStringLiteral("'"));
            return nullptr;
        }

        QStringList originales;
        QStringList normales;
        while (!fin()) {
            const QString normal = actualNormal();
            if (normal == QStringLiteral("(") || normal == QStringLiteral(")")
                || esOperador(normal)) break;
            originales.append(actualOriginal());
            normales.append(normal);
            ++m_pos;
        }
        if (normales.isEmpty()) {
            errorInterno(QStringLiteral("Se esperaba una proposición"));
            return nullptr;
        }
        return Formula::crearAtomo(normales.join(QStringLiteral(" ")), originales.join(QStringLiteral(" ")));
    }
};

} // namespace

bool AnalizadorSemantico::analizarLinea(const QString& linea, FormulaPtr& formula, QString& error) const {
    if (linea.trimmed().isEmpty()) {
        error = QStringLiteral("Línea vacía.");
        return false;
    }
    const QVector<Token> tokens = tokenizar(linea);
    if (tokens.isEmpty()) {
        error = QStringLiteral("Línea vacía.");
        return false;
    }
    ParserInterno parser(tokens);
    formula = parser.parse(error);
    return formula != nullptr;
}

bool AnalizadorSemantico::analizarBase(const QString& texto, QVector<FormulaRegla>& reglas, QString& error) const {
    reglas.clear();
    const QStringList lineas = texto.split(QLatin1Char('\n'));
    for (int i = 0; i < lineas.size(); ++i) {
        const QString linea = lineas[i];
        if (linea.trimmed().isEmpty()) {
            error = QStringLiteral("Línea %1: línea vacía.").arg(i + 1);
            return false;
        }
        FormulaPtr formula;
        QString errorLinea;
        if (!analizarLinea(linea, formula, errorLinea)) {
            error = QStringLiteral("Línea %1: %2").arg(i + 1).arg(errorLinea);
            return false;
        }
        reglas.append(FormulaRegla{linea.trimmed(), formula});
    }
    if (reglas.isEmpty()) {
        error = QStringLiteral("No hay reglas para analizar.");
        return false;
    }
    return true;
}