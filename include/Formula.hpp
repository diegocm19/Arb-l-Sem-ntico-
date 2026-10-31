#pragma once

#include <QString>
#include <memory>

enum class TipoFormula { Atomo, Negacion, Conjuncion, Disyuncion, Implicacion, Bicondicional };

class Formula;
using FormulaPtr = std::shared_ptr<Formula>;

class Formula {
public:
    explicit Formula(TipoFormula tipo);

    TipoFormula tipo;
    QString atomoClave;
    QString atomoTexto;
    QString atomoLetra;
    FormulaPtr izquierda;
    FormulaPtr derecha;

    static FormulaPtr crearAtomo(const QString& clave, const QString& texto);
    static FormulaPtr crearNegacion(const FormulaPtr& formula);
    static FormulaPtr crearConjuncion(const FormulaPtr& izquierda, const FormulaPtr& derecha);
    static FormulaPtr crearDisyuncion(const FormulaPtr& izquierda, const FormulaPtr& derecha);
    static FormulaPtr crearImplicacion(const FormulaPtr& izquierda, const FormulaPtr& derecha);
    static FormulaPtr crearBicondicional(const FormulaPtr& izquierda, const FormulaPtr& derecha);

    bool esAtomo() const;
    bool esLiteral() const;
    bool esNegacionDeAtomo() const;
    QString toString(int parentPrec = -1) const;
    int precedencia() const;
};
