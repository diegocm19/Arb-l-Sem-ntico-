#include "MotorArbol.hpp"

ResultadoMotor MotorArbol::construir(const QVector<FormulaPtr>& formulas) {
    ResultadoMotor resultado;

    m_siguienteId = 0;
    m_raiz = std::make_shared<NodoArbol>();
    m_raiz->id = m_siguienteId++;
    for (const FormulaPtr& f : formulas) m_raiz->agregarFormula(f);

    while (true) {
        QVector<NodoPtr> hojas;
        recolectarHojas(m_raiz, hojas);

        bool seCerroAlguna = false;
        for (const NodoPtr& hoja : hojas) {
            if (hoja->cerrada || hoja->abierta) continue;
            QString detalle;
            if (hoja->contieneContradiccion(&detalle)) {
                hoja->cerrada = true;
                hoja->contradiccion = detalle;
                seCerroAlguna = true;
            }
        }
        if (seCerroAlguna) continue;

        NodoPtr hojaNoRamifica = buscarHojaConPendiente(true);
        if (hojaNoRamifica) { expandir(hojaNoRamifica); continue; }

        NodoPtr hojaRamifica = buscarHojaConPendiente(false);
        if (hojaRamifica) { expandir(hojaRamifica); continue; }

        hojas.clear();
        recolectarHojas(m_raiz, hojas);
        for (const NodoPtr& hoja : hojas) {
            if (!hoja->cerrada && !hoja->abierta) hoja->abierta = true;
        }
        break;
    }

    resultado.raiz = m_raiz;
    contar(m_raiz, resultado.ramasCerradas, resultado.ramasAbiertas);
    return resultado;
}

bool MotorArbol::expandir(const NodoPtr& nodo) {
    int indice = indicePendiente(nodo, true);
    if (indice < 0) indice = indicePendiente(nodo, false);
    if (indice < 0) return false;

    const FormulaPtr formula = nodo->entradas[indice].formula;
    nodo->entradas[indice].aplicada = true;

    const QVector<QVector<FormulaPtr>> partes = descomponer(formula);
    for (const QVector<FormulaPtr>& parte : partes) {
        auto hijo = std::make_shared<NodoArbol>();
        hijo->id = m_siguienteId++;
        hijo->padre = nodo.get();
        hijo->entradas = nodo->entradas;
        for (const FormulaPtr& nueva : parte) hijo->agregarFormula(nueva);
        nodo->hijos.push_back(hijo);
    }
    return true;
}

NodoPtr MotorArbol::buscarHojaConPendiente(bool soloNoRamifica) const {
    QVector<NodoPtr> hojas;
    recolectarHojas(m_raiz, hojas);
    for (const NodoPtr& hoja : hojas) {
        if (hoja->cerrada || hoja->abierta) continue;
        if (indicePendiente(hoja, soloNoRamifica) >= 0) return hoja;
    }
    return nullptr;
}

int MotorArbol::indicePendiente(const NodoPtr& nodo, bool soloNoRamifica) const {
    for (int i = 0; i < static_cast<int>(nodo->entradas.size()); ++i) {
        const EntradaFormula& e = nodo->entradas[i];
        if (e.aplicada || !e.formula || e.formula->esLiteral()) continue;
        if (soloNoRamifica) {
            if (esNoRamifica(e.formula)) return i;
        } else {
            if (esRamifica(e.formula)) return i;
        }
    }
    return -1;
}

QVector<QVector<FormulaPtr>> MotorArbol::descomponer(const FormulaPtr& formula) const {
    QVector<QVector<FormulaPtr>> partes;
    if (!formula) return partes;

    switch (formula->tipo) {
    case TipoFormula::Atomo:
        break;

    case TipoFormula::Conjuncion:
        partes.append(QVector<FormulaPtr>{formula->izquierda, formula->derecha});
        break;

    case TipoFormula::Disyuncion:
        partes.append(QVector<FormulaPtr>{formula->izquierda});
        partes.append(QVector<FormulaPtr>{formula->derecha});
        break;

    case TipoFormula::Implicacion:
        partes.append(QVector<FormulaPtr>{Formula::crearNegacion(formula->izquierda)});
        partes.append(QVector<FormulaPtr>{formula->derecha});
        break;

    case TipoFormula::Bicondicional:
        partes.append(QVector<FormulaPtr>{formula->izquierda, formula->derecha});
        partes.append(QVector<FormulaPtr>{Formula::crearNegacion(formula->izquierda),
                                          Formula::crearNegacion(formula->derecha)});
        break;

    case TipoFormula::Negacion: {
        const FormulaPtr& interior = formula->izquierda;
        if (!interior) break;
        switch (interior->tipo) {
        case TipoFormula::Negacion:
            partes.append(QVector<FormulaPtr>{interior->izquierda});
            break;
        case TipoFormula::Conjuncion:
            partes.append(QVector<FormulaPtr>{Formula::crearNegacion(interior->izquierda)});
            partes.append(QVector<FormulaPtr>{Formula::crearNegacion(interior->derecha)});
            break;
        case TipoFormula::Disyuncion:
            partes.append(QVector<FormulaPtr>{Formula::crearNegacion(interior->izquierda),
                                              Formula::crearNegacion(interior->derecha)});
            break;
        case TipoFormula::Implicacion:
            partes.append(QVector<FormulaPtr>{interior->izquierda,
                                              Formula::crearNegacion(interior->derecha)});
            break;
        case TipoFormula::Bicondicional:
            partes.append(QVector<FormulaPtr>{interior->izquierda,
                                              Formula::crearNegacion(interior->derecha)});
            partes.append(QVector<FormulaPtr>{Formula::crearNegacion(interior->izquierda),
                                              interior->derecha});
            break;
        case TipoFormula::Atomo:
            break;
        }
        break;
    }
    }
    return partes;
}

bool MotorArbol::esNoRamifica(const FormulaPtr& formula) const {
    if (!formula) return false;
    if (formula->tipo == TipoFormula::Conjuncion) return true;
    if (formula->tipo == TipoFormula::Negacion && formula->izquierda) {
        const TipoFormula t = formula->izquierda->tipo;
        if (t == TipoFormula::Negacion) return true;
        if (t == TipoFormula::Disyuncion) return true;
        if (t == TipoFormula::Implicacion) return true;
    }
    return false;
}

bool MotorArbol::esRamifica(const FormulaPtr& formula) const {
    if (!formula) return false;
    if (formula->tipo == TipoFormula::Disyuncion) return true;
    if (formula->tipo == TipoFormula::Implicacion) return true;
    if (formula->tipo == TipoFormula::Bicondicional) return true;
    if (formula->tipo == TipoFormula::Negacion && formula->izquierda) {
        const TipoFormula t = formula->izquierda->tipo;
        if (t == TipoFormula::Conjuncion) return true;
        if (t == TipoFormula::Bicondicional) return true;
    }
    return false;
}

void MotorArbol::recolectarHojas(const NodoPtr& nodo, QVector<NodoPtr>& hojas) const {
    if (!nodo) return;
    if (nodo->esHoja()) { hojas.append(nodo); return; }
    for (const NodoPtr& h : nodo->hijos) recolectarHojas(h, hojas);
}

void MotorArbol::contar(const NodoPtr& nodo, int& cerradas, int& abiertas) const {
    if (!nodo) return;
    if (nodo->esHoja()) {
        if (nodo->cerrada) ++cerradas;
        else if (nodo->abierta) ++abiertas;
        return;
    }
    for (const NodoPtr& h : nodo->hijos) contar(h, cerradas, abiertas);
}