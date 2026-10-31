#pragma once

#include <QString>
#include <memory>
#include <vector>

#include "Formula.hpp"

struct EntradaFormula {
    FormulaPtr formula;
    bool aplicada = false;
};

class NodoArbol;
using NodoPtr = std::shared_ptr<NodoArbol>;

class NodoArbol {
public:
    int id = 0;
    NodoArbol* padre = nullptr;
    std::vector<NodoPtr> hijos;
    std::vector<EntradaFormula> entradas;
    bool cerrada = false;
    bool abierta = false;
    QString contradiccion;

    bool esHoja() const { return hijos.empty(); }

    void agregarFormula(const FormulaPtr& formula) {
        if (formula) entradas.push_back(EntradaFormula{formula, false});
    }

    bool contieneContradiccion(QString* detalle = nullptr) const;
};