#include "RegistroAtomos.hpp"

void RegistroAtomos::limpiar() {
    m_atomos.clear();
    m_letras.clear();
    m_siguiente = 0;
}

QString RegistroAtomos::siguienteLetra() {
    const int n = m_siguiente++;
    if (n < 26) {
        return QString(QChar(static_cast<ushort>('p' + n)));
    }
    const int base = n - 26;
    return QString(QChar(static_cast<ushort>('p' + base % 26))) + QString::number(base / 26 + 1);
}

void RegistroAtomos::registrarFormula(const FormulaPtr& formula) {
    if (!formula) return;

    if (formula->tipo == TipoFormula::Atomo) {
        if (!m_letras.contains(formula->atomoClave)) {
            const QString letra = siguienteLetra();
            m_letras.insert(formula->atomoClave, letra);
            m_atomos.append(Atomo{letra, formula->atomoTexto, formula->atomoClave});
        }
        formula->atomoLetra = m_letras.value(formula->atomoClave);
        return;
    }

    registrarFormula(formula->izquierda);
    registrarFormula(formula->derecha);
}

QVector<Atomo> RegistroAtomos::atomos() const { return m_atomos; }
QString RegistroAtomos::letraDe(const QString& clave) const { return m_letras.value(clave); }