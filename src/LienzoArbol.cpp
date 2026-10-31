#include "LienzoArbol.hpp"

#include <QFontMetrics>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QScrollBar>
#include <QWheelEvent>

namespace {
const QFont FUENTE_FORMULA(QStringLiteral("Consolas"), 9);
const QFont FUENTE_UI(QStringLiteral("Segoe UI"), 9);
}

LienzoArbol::LienzoArbol(QWidget* parent) : QGraphicsView(parent) {
    m_escena = new QGraphicsScene(this);
    setScene(m_escena);
    setRenderHint(QPainter::Antialiasing, true);
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::NoAnchor);
    setBackgroundBrush(QColor(9, 11, 16));
    setFont(FUENTE_UI);
    setMinimumSize(320, 280);
}

void LienzoArbol::mostrarArbol(const NodoPtr& raiz) {
    limpiar();
    if (!raiz) return;

    NodoGrafico* grafico = construirGrafico(raiz);
    contarHojas(grafico);
    double siguienteY = MARGEN + 95.0;
    posicionar(grafico, 0, siguienteY);
    dibujar(grafico);
    liberar(grafico);
    ajustarVista();
}

void LienzoArbol::limpiar() {
    m_escena->clear();
    resetTransform();
    m_zoom = 1.0;
    m_arrastrando = false;
    unsetCursor();
}

void LienzoArbol::ajustarVista() {
    const QRectF limites = m_escena->itemsBoundingRect();
    if (limites.isEmpty()) return;
    const QRectF ajustados = limites.adjusted(-MARGEN, -MARGEN, MARGEN, MARGEN);
    fitInView(ajustados, Qt::KeepAspectRatio);
    m_zoom = transform().m11();
}

void LienzoArbol::wheelEvent(QWheelEvent* evento) {
    if (evento->angleDelta().y() == 0) {
        QGraphicsView::wheelEvent(evento);
        return;
    }

    const double factor = evento->angleDelta().y() > 0 ? 1.15 : 1.0 / 1.15;
    const double nuevoZoom = m_zoom * factor;
    if (nuevoZoom < 0.1 || nuevoZoom > 5.0) {
        evento->accept();
        return;
    }
    m_zoom = nuevoZoom;
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    scale(factor, factor);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    evento->accept();
}

void LienzoArbol::mousePressEvent(QMouseEvent* evento) {
    if (evento->button() == Qt::LeftButton) {
        m_arrastrando = true;
        m_ultimoPunto = evento->pos();
        setCursor(Qt::ClosedHandCursor);
        evento->accept();
        return;
    }
    QGraphicsView::mousePressEvent(evento);
}

void LienzoArbol::mouseMoveEvent(QMouseEvent* evento) {
    if (m_arrastrando) {
        const QPoint delta = evento->pos() - m_ultimoPunto;
        m_ultimoPunto = evento->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        evento->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(evento);
}

void LienzoArbol::mouseReleaseEvent(QMouseEvent* evento) {
    if (evento->button() == Qt::LeftButton && m_arrastrando) {
        m_arrastrando = false;
        unsetCursor();
        evento->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(evento);
}

LienzoArbol::NodoGrafico* LienzoArbol::construirGrafico(const NodoPtr& nodo) {
    auto* grafico = new NodoGrafico();
    grafico->nodo = nodo;

    const size_t heredadas = nodo->padre
        ? static_cast<size_t>(nodo->padre->entradas.size())
        : 0;
    for (size_t i = heredadas; i < nodo->entradas.size(); ++i) {
        const EntradaFormula& entrada = nodo->entradas[i];
        if (entrada.formula) grafico->formulasNuevas.append(entrada.formula->toString());
    }

    for (const NodoPtr& hijo : nodo->hijos) {
        grafico->hijos.append(construirGrafico(hijo));
    }
    return grafico;
}

QSizeF LienzoArbol::tamanoNodo(const NodoGrafico* grafico) const {
    const int filas = qMax(1, grafico->formulasNuevas.size());
    const qreal alto = 64.0 + filas * ALTO_LINEA
        + ((grafico->nodo->cerrada || grafico->nodo->abierta) ? 32.0 : 0.0);
    return QSizeF(ANCHO_NODO, alto);
}

int LienzoArbol::contarHojas(NodoGrafico* nodo) {
    if (nodo->hijos.isEmpty()) {
        nodo->hojas = 1;
        return nodo->hojas;
    }

    nodo->hojas = 0;
    for (NodoGrafico* hijo : nodo->hijos) nodo->hojas += contarHojas(hijo);
    return nodo->hojas;
}

void LienzoArbol::posicionar(NodoGrafico* nodo, int profundidad, double& siguienteY) {
    nodo->profundidad = profundidad;
    if (nodo->hijos.isEmpty()) {
        nodo->centroY = siguienteY;
        siguienteY += qMax(210.0, tamanoNodo(nodo).height() + SEPARACION_V);
    } else {
        for (NodoGrafico* hijo : nodo->hijos) posicionar(hijo, profundidad + 1, siguienteY);
        nodo->centroY = (nodo->hijos.first()->centroY + nodo->hijos.last()->centroY) / 2.0;
    }

    const double x = MARGEN + profundidad * (ANCHO_NODO + SEPARACION_H);
    const QSizeF size = tamanoNodo(nodo);
    nodo->rect = QRectF(x, nodo->centroY - size.height() / 2.0, size.width(), size.height());
}

void LienzoArbol::dibujar(NodoGrafico* nodo) {
    for (NodoGrafico* hijo : nodo->hijos) {
        const QPointF inicio(nodo->rect.right(), nodo->rect.center().y());
        const QPointF fin(hijo->rect.left(), hijo->rect.center().y());
        const qreal control = (fin.x() - inicio.x()) * 0.48;
        QPainterPath ruta(inicio);
        ruta.cubicTo(inicio.x() + control, inicio.y(),
                     fin.x() - control, fin.y(), fin.x(), fin.y());
        QPen lapiz(QColor(153, 165, 190), 2.0);
        m_escena->addPath(ruta, lapiz);

        const QPointF punta = fin - QPointF(7.0, 0.0);
        QPainterPath flecha;
        flecha.moveTo(punta);
        flecha.lineTo(punta + QPointF(-7.0, -4.5));
        flecha.lineTo(punta + QPointF(-7.0, 4.5));
        flecha.closeSubpath();
        m_escena->addPath(flecha, QPen(Qt::NoPen), QBrush(QColor(153, 165, 190)));
        dibujar(hijo);
    }

    QColor acento(214, 69, 69);
    QColor relleno(24, 17, 20);
    QString estado = QStringLiteral("EXPANSIÓN");
    if (nodo->nodo->cerrada) {
        acento = QColor(50, 106, 192);
        relleno = QColor(17, 28, 46);
        estado = QStringLiteral("RAMA CERRADA");
    } else if (nodo->nodo->abierta) {
        acento = QColor(30, 155, 117);
        relleno = QColor(15, 31, 26);
        estado = QStringLiteral("MODELO ABIERTO");
    } else if (nodo->hijos.isEmpty()) {
        estado = QStringLiteral("EN PROCESO");
    }

    QPainterPath tarjeta;
    tarjeta.addRoundedRect(nodo->rect, 13.0, 13.0);
    m_escena->addPath(tarjeta, QPen(QColor(47, 54, 68), 1.2), QBrush(relleno));
    const QRectF barra(nodo->rect.left(), nodo->rect.top() + 11.0, 4.0, nodo->rect.height() - 22.0);
    QPainterPath formaBarra;
    formaBarra.addRoundedRect(barra, 2.0, 2.0);
    m_escena->addPath(formaBarra, QPen(Qt::NoPen), QBrush(acento));

    auto* cabecera = m_escena->addSimpleText(
        QStringLiteral("PASO  %1").arg(nodo->nodo->id, 2, 10, QLatin1Char('0')), FUENTE_UI);
    cabecera->setBrush(QColor(235, 238, 245));
    cabecera->setPos(nodo->rect.left() + 16.0, nodo->rect.top() + 10.0);

    auto* etiqueta = m_escena->addSimpleText(estado, FUENTE_UI);
    etiqueta->setBrush(acento);
    etiqueta->setPos(nodo->rect.right() - etiqueta->boundingRect().width() - 13.0,
                     nodo->rect.top() + 11.0);

    QPen separador(QColor(48, 55, 68));
    m_escena->addLine(nodo->rect.left() + 14.0, nodo->rect.top() + 37.0,
                      nodo->rect.right() - 14.0, nodo->rect.top() + 37.0, separador);

    QFont fuenteFormula = FUENTE_FORMULA;
    const QFontMetrics metricas(fuenteFormula);
    const int maxAncho = static_cast<int>(ANCHO_NODO - 34.0);
    double y = nodo->rect.top() + 45.0;
    if (nodo->formulasNuevas.isEmpty()) {
        auto* texto = m_escena->addSimpleText(QStringLiteral("Sin fórmulas nuevas"), FUENTE_UI);
        texto->setBrush(QColor(151, 160, 177));
        texto->setPos(nodo->rect.left() + 16.0, y);
    } else {
        for (const QString& formula : nodo->formulasNuevas) {
            QString visible = metricas.elidedText(formula, Qt::ElideRight, maxAncho - 22);
            auto* texto = m_escena->addSimpleText(QStringLiteral("›  ") + visible, fuenteFormula);
            texto->setBrush(QColor(222, 227, 237));
            texto->setToolTip(formula);
            texto->setPos(nodo->rect.left() + 16.0, y);
            y += ALTO_LINEA;
        }
    }

    if (nodo->nodo->cerrada || nodo->nodo->abierta) {
        const QString resultado = nodo->nodo->cerrada
            ? nodo->nodo->contradiccion
            : QStringLiteral("Rama satisfacible");
        auto* detalle = m_escena->addSimpleText(
            metricas.elidedText(resultado, Qt::ElideRight, maxAncho), FUENTE_UI);
        detalle->setBrush(nodo->nodo->cerrada ? QColor(50, 106, 192) : QColor(25, 132, 96));
        detalle->setToolTip(resultado);
        detalle->setPos(nodo->rect.left() + 16.0,
                        nodo->rect.bottom() - detalle->boundingRect().height() - 10.0);
    }
}

void LienzoArbol::liberar(NodoGrafico* nodo) {
    if (!nodo) return;
    for (NodoGrafico* hijo : nodo->hijos) liberar(hijo);
    delete nodo;
}
