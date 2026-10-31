#include "NodoArbol.hpp"
#include <QMap>
#include <QSet>
bool NodoArbol::contieneContradiccion(QString* detalle) const {
    QSet<QString> positivos, negativos;
    QMap<QString,QString> letras;
    for (const auto& e : entradas) {
        if (!e.formula) continue;
        if (e.formula->tipo == TipoFormula::Atomo) {
            positivos.insert(e.formula->atomoClave);
            letras[e.formula->atomoClave] = e.formula->atomoLetra.isEmpty()? e.formula->atomoTexto : e.formula->atomoLetra;
        } else if (e.formula->tipo == TipoFormula::Negacion && e.formula->izquierda && e.formula->izquierda->tipo == TipoFormula::Atomo) {
            const auto& a = e.formula->izquierda;
            negativos.insert(a->atomoClave);
            letras[a->atomoClave] = a->atomoLetra.isEmpty()? a->atomoTexto : a->atomoLetra;
        }
    }
    for (const auto& clave : positivos) {
        if (negativos.contains(clave)) {
            if (detalle) {
                QString letra = letras.value(clave, clave);
                *detalle = letra + " y ¬" + letra;
            }
            return true;
        }
    }
    return false;
}