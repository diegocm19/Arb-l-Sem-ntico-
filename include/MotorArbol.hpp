#pragma once
#include <QVector>
#include "NodoArbol.hpp"
struct ResultadoMotor {
    NodoPtr raiz;
    int ramasCerradas = 0;
    int ramasAbiertas = 0;
};
class MotorArbol {
public:
    ResultadoMotor construir(const QVector<FormulaPtr>& formulas);
private:
    NodoPtr m_raiz;
    int m_siguienteId = 0;
    bool expandir(const NodoPtr& nodo);
    NodoPtr buscarHojaConPendiente(bool soloNoRamifica) const;
    bool tienePendienteNoRamifica(const NodoPtr& nodo) const;
    bool tienePendienteRamifica(const NodoPtr& nodo) const;
    int indicePendiente(const NodoPtr& nodo, bool soloNoRamifica) const;
    QVector<QVector<FormulaPtr>> descomponer(const FormulaPtr& f) const;
    bool esNoRamifica(const FormulaPtr& f) const;
    bool esRamifica(const FormulaPtr& f) const;
    void recolectarHojas(const NodoPtr& nodo, QVector<NodoPtr>& hojas) const;
    void contar(const NodoPtr& nodo, int& cerradas, int& abiertas) const;
};