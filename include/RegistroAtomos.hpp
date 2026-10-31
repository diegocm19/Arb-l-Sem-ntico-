#pragma once

#include <QMap>
#include <QString>
#include <QVector>

#include "Atomo.hpp"
#include "Formula.hpp"

class RegistroAtomos {
public:
    void limpiar();
    void registrarFormula(const FormulaPtr& formula);
    QVector<Atomo> atomos() const;
    QString letraDe(const QString& clave) const;

private:
    QVector<Atomo> m_atomos;
    QMap<QString, QString> m_letras;
    int m_siguiente = 0;

    QString siguienteLetra();
};