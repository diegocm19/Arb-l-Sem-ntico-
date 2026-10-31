#pragma once

#include <QGraphicsView>
#include <QPoint>
#include "MotorArbol.hpp"

class QGraphicsScene;
class QMouseEvent;
class QWheelEvent;

class LienzoArbol : public QGraphicsView {
public:
    explicit LienzoArbol(QWidget* parent = nullptr);
    void mostrarArbol(const NodoPtr& raiz);
    void limpiar();

protected:
    void wheelEvent(QWheelEvent* evento) override;
    void mousePressEvent(QMouseEvent* evento) override;
    void mouseMoveEvent(QMouseEvent* evento) override;
    void mouseReleaseEvent(QMouseEvent* evento) override;

private:
    struct NodoGrafico {
        NodoPtr nodo;
        QVector<NodoGrafico*> hijos;
        int hojas = 0;
        double centroY = 0.0;
        int profundidad = 0;
        QRectF rect;
        QStringList formulasNuevas;
    };

    static constexpr double MARGEN = 40.0;
    static constexpr double ANCHO_NODO = 260.0;
    static constexpr double SEPARACION_H = 115.0;
    static constexpr double SEPARACION_V = 48.0;
    static constexpr double ALTO_LINEA = 24.0;

    QGraphicsScene* m_escena = nullptr;
    double m_zoom = 1.0;
    bool m_arrastrando = false;
    QPoint m_ultimoPunto;

    NodoGrafico* construirGrafico(const NodoPtr& nodo);
    QSizeF tamanoNodo(const NodoGrafico* nodo) const;
    int contarHojas(NodoGrafico* nodo);
    void posicionar(NodoGrafico* nodo, int profundidad, double& siguienteY);
    void dibujar(NodoGrafico* nodo);
    void liberar(NodoGrafico* nodo);
    void ajustarVista();
};
