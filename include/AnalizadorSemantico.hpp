#pragma once

#include <QString>
#include <QVector>
#include "Formula.hpp"

struct FormulaRegla {
    QString textoOriginal;
    FormulaPtr formula;
};

class AnalizadorSemantico {
public:
    bool analizarLinea(const QString& linea, FormulaPtr& formula, QString& error) const;
    bool analizarBase(const QString& texto, QVector<FormulaRegla>& reglas, QString& error) const;
};
