#include "Formula.hpp"

Formula::Formula(TipoFormula tipoFormula) : tipo(tipoFormula) {}

FormulaPtr Formula::crearAtomo(const QString& clave, const QString& texto) {
    auto formula = std::make_shared<Formula>(TipoFormula::Atomo);
    formula->atomoClave = clave;
    formula->atomoTexto = texto;
    return formula;
}

FormulaPtr Formula::crearNegacion(const FormulaPtr& formula) {
    auto resultado = std::make_shared<Formula>(TipoFormula::Negacion);
    resultado->izquierda = formula;
    return resultado;
}

FormulaPtr Formula::crearConjuncion(const FormulaPtr& izquierda, const FormulaPtr& derecha) {
    auto formula = std::make_shared<Formula>(TipoFormula::Conjuncion);
    formula->izquierda = izquierda;
    formula->derecha = derecha;
    return formula;
}

FormulaPtr Formula::crearDisyuncion(const FormulaPtr& izquierda, const FormulaPtr& derecha) {
    auto formula = std::make_shared<Formula>(TipoFormula::Disyuncion);
    formula->izquierda = izquierda;
    formula->derecha = derecha;
    return formula;
}

FormulaPtr Formula::crearImplicacion(const FormulaPtr& izquierda, const FormulaPtr& derecha) {
    auto formula = std::make_shared<Formula>(TipoFormula::Implicacion);
    formula->izquierda = izquierda;
    formula->derecha = derecha;
    return formula;
}

FormulaPtr Formula::crearBicondicional(const FormulaPtr& izquierda, const FormulaPtr& derecha) {
    auto formula = std::make_shared<Formula>(TipoFormula::Bicondicional);
    formula->izquierda = izquierda;
    formula->derecha = derecha;
    return formula;
}

bool Formula::esAtomo() const {
    return tipo == TipoFormula::Atomo;
}

bool Formula::esNegacionDeAtomo() const {
    return tipo == TipoFormula::Negacion && izquierda && izquierda->esAtomo();
}

bool Formula::esLiteral() const {
    return esAtomo() || esNegacionDeAtomo();
}

int Formula::precedencia() const {
    switch (tipo) {
    case TipoFormula::Bicondicional: return 0;
    case TipoFormula::Implicacion:   return 1;
    case TipoFormula::Disyuncion:    return 2;
    case TipoFormula::Conjuncion:    return 3;
    case TipoFormula::Negacion:      return 4;
    case TipoFormula::Atomo:         return 5;
    }
    return 5;
}

QString Formula::toString(int parentPrec) const {
    QString resultado;
    const int prec = precedencia();

    switch (tipo) {
    case TipoFormula::Atomo:
        resultado = atomoLetra.isEmpty() ? atomoTexto : atomoLetra;
        break;
    case TipoFormula::Negacion:
        resultado = QStringLiteral("¬") + (izquierda ? izquierda->toString(4) : QStringLiteral("?"));
        break;
    case TipoFormula::Conjuncion:
        resultado = izquierda->toString(3) + QStringLiteral(" ∧ ") + derecha->toString(4);
        break;
    case TipoFormula::Disyuncion:
        resultado = izquierda->toString(2) + QStringLiteral(" ∨ ") + derecha->toString(3);
        break;
    case TipoFormula::Implicacion:
        resultado = izquierda->toString(2) + QStringLiteral(" → ") + derecha->toString(1);
        break;
    case TipoFormula::Bicondicional:
        resultado = izquierda->toString(1) + QStringLiteral(" ↔ ") + derecha->toString(0);
        break;
    }

    if (parentPrec >= 0 && prec < parentPrec) {
        resultado = QStringLiteral("(") + resultado + QStringLiteral(")");
    }
    return resultado;
}
