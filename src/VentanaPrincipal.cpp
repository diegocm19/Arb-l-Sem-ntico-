#include "VentanaPrincipal.hpp"

#include <QFrame>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QMap>
#include <QStringList>
#include <functional>

VentanaPrincipal::VentanaPrincipal(QWidget* parent) : QMainWindow(parent) {
    configurarUi();
    cargarEjemplo();
}

void VentanaPrincipal::configurarUi() {
    setWindowTitle(QStringLiteral("Semántico | Laboratorio lógico"));
    resize(1360, 860);
    setMinimumSize(980, 680);

    setStyleSheet(QStringLiteral(R"(
        QMainWindow, QWidget#central { background: #080a0f; color: #f1f3f8; }
        QLabel { color: #e0e4ed; }
        QFrame#topBar { background: #11141c; border: 1px solid #272d3a; border-radius: 18px; }
        QLabel#brandMark { background: #d64545; color: #ffffff; border-radius: 14px; font-size: 22px; font-weight: 800; }
        QLabel#appTitle { color: #f4f5f8; font-size: 21px; font-weight: 800; }
        QLabel#appSubtitle { color: #9299a8; font-size: 11px; }
        QLabel#topBadge { background: #30191d; color: #ff8585; border-radius: 10px; padding: 8px 12px; font-size: 10px; font-weight: 800; }
        QFrame#inputPanel, QFrame#summaryPanel, QTabWidget::pane { background: #11141c; border: 1px solid #272d3a; border-radius: 16px; }
        QLabel#sectionTitle { color: #f0f2f7; font-size: 15px; font-weight: 800; }
        QLabel#sectionHint, QLabel#helperText { color: #9aa2b2; font-size: 11px; }
        QLabel#sectionIndex { color: #ff8585; background: #30191d; border-radius: 9px; padding: 5px 9px; font-size: 10px; font-weight: 800; }
        QTextEdit#rulesEditor, QTextEdit#resultsView, QTableWidget, LienzoArbol {
            background: #090b10; color: #e5e8ef; border: 1px solid #2a303d; border-radius: 11px;
            selection-background-color: #632b33; selection-color: #ffffff;
        }
        QTextEdit#rulesEditor { padding: 12px; font-family: Consolas, 'Cascadia Code', monospace; font-size: 12px; }
        QTextEdit#resultsView { padding: 14px; font-size: 13px; }
        QPushButton { border: 0; border-radius: 10px; padding: 10px 15px; font-weight: 700; }
        QPushButton#analyzeButton { background: #d64545; color: #ffffff; }
        QPushButton#analyzeButton:hover { background: #b93434; }
        QPushButton#exampleButton { background: #142a29; color: #58c9b0; }
        QPushButton#exampleButton:hover { background: #1b3a37; }
        QFrame#statusCard { background: #211519; border: 1px solid #48242a; border-radius: 12px; }
        QLabel#statusEyebrow { color: #c18489; font-size: 10px; font-weight: 800; }
        QLabel#resultStatus { color: #ff8585; font-size: 17px; font-weight: 800; }
        QFrame#openCard { background: #10251f; border: 1px solid #1e493b; border-radius: 12px; }
        QFrame#closedCard { background: #111e32; border: 1px solid #263d60; border-radius: 12px; }
        QLabel#openCount { color: #53d3a4; font-size: 23px; font-weight: 800; }
        QLabel#closedCount { color: #77aaff; font-size: 23px; font-weight: 800; }
        QLabel#countCaption { color: #a3aaba; font-size: 10px; font-weight: 700; }
        QTabBar::tab { background: #1a1e28; color: #b0b7c5; border: 0; border-radius: 9px; padding: 9px 14px; margin: 4px 3px; font-weight: 700; }
        QTabBar::tab:selected { background: #d64545; color: #ffffff; }
        QTabBar::tab:hover:!selected { background: #292e3a; color: #ffffff; }
        QHeaderView::section { background: #1a1e28; color: #c4cad5; border: 0; border-bottom: 1px solid #303746; padding: 10px; font-weight: 800; }
        QTableWidget { gridline-color: #252b36; alternate-background-color: #10131a; }
        QTableWidget#detailTable { font-size: 11px; }
        QTableWidget::item, QTreeWidget::item { color: #e0e4ed; }
        QScrollBar:vertical, QScrollBar:horizontal { background: #0b0d12; border: 0; }
        QScrollBar::handle:vertical, QScrollBar::handle:horizontal { background: #3a414f; border-radius: 4px; min-height: 24px; min-width: 24px; }
        QScrollBar::handle:vertical:hover, QScrollBar::handle:horizontal:hover { background: #596275; }
        QStatusBar { background: #11141c; color: #a4abba; border-top: 1px solid #272d3a; }
        QSplitter::handle { background: transparent; width: 10px; }
    )"));

    auto* central = new QWidget(this);
    central->setObjectName(QStringLiteral("central"));
    auto* principal = new QVBoxLayout(central);
    principal->setContentsMargins(22, 18, 22, 12);
    principal->setSpacing(15);

    auto* cabecera = new QFrame(central);
    cabecera->setObjectName(QStringLiteral("topBar"));
    auto* layoutCabecera = new QHBoxLayout(cabecera);
    layoutCabecera->setContentsMargins(18, 14, 18, 14);
    layoutCabecera->setSpacing(13);
    auto* marca = new QLabel(QStringLiteral("∴"), cabecera);
    marca->setObjectName(QStringLiteral("brandMark"));
    marca->setAlignment(Qt::AlignCenter);
    marca->setFixedSize(46, 46);
    auto* textosCabecera = new QVBoxLayout();
    textosCabecera->setSpacing(2);
    auto* titulo = new QLabel(QStringLiteral("Semántico"), cabecera);
    titulo->setObjectName(QStringLiteral("appTitle"));
    auto* subtitulo = new QLabel(QStringLiteral("LABORATORIO DE LÓGICA PROPOSICIONAL"), cabecera);
    subtitulo->setObjectName(QStringLiteral("appSubtitle"));
    textosCabecera->addWidget(titulo);
    textosCabecera->addWidget(subtitulo);
    auto* insignia = new QLabel(QStringLiteral("INFERENCIA · ESPAÑOL"), cabecera);
    insignia->setObjectName(QStringLiteral("topBadge"));
    layoutCabecera->addWidget(marca);
    layoutCabecera->addLayout(textosCabecera);
    layoutCabecera->addStretch();
    layoutCabecera->addWidget(insignia);
    principal->addWidget(cabecera);

    auto* divisor = new QSplitter(Qt::Horizontal, central);
    divisor->setChildrenCollapsible(false);
    divisor->setHandleWidth(10);

    auto* panelEntrada = new QFrame(divisor);
    panelEntrada->setObjectName(QStringLiteral("inputPanel"));
    panelEntrada->setMinimumWidth(310);
    auto* layoutEntrada = new QVBoxLayout(panelEntrada);
    layoutEntrada->setContentsMargins(18, 20, 18, 18);
    layoutEntrada->setSpacing(12);

    auto* tituloEntradaFila = new QHBoxLayout();
    auto* tituloEntrada = new QLabel(QStringLiteral("Base de conocimiento"), panelEntrada);
    tituloEntrada->setObjectName(QStringLiteral("sectionTitle"));
    auto* indiceEntrada = new QLabel(QStringLiteral("01 / ENTRADA"), panelEntrada);
    indiceEntrada->setObjectName(QStringLiteral("sectionIndex"));
    tituloEntradaFila->addWidget(tituloEntrada);
    tituloEntradaFila->addStretch();
    tituloEntradaFila->addWidget(indiceEntrada);
    auto* ayudaEntrada = new QLabel(QStringLiteral("Escribe una proposición por línea. El analizador construirá el tableau semántico."), panelEntrada);
    ayudaEntrada->setObjectName(QStringLiteral("sectionHint"));
    ayudaEntrada->setWordWrap(true);
    textoReglas = new QTextEdit(panelEntrada);
    textoReglas->setObjectName(QStringLiteral("rulesEditor"));
    textoReglas->setPlaceholderText(QStringLiteral("si llueve entonces suelo mojado\nllueve\nno suelo mojado"));
    textoReglas->setAcceptRichText(false);
    textoReglas->setTabChangesFocus(false);
    auto* notaSintaxis = new QLabel(QStringLiteral("Operadores:  no  ·  y  ·  o  ·  si ... entonces  ·  si y solo si"), panelEntrada);
    notaSintaxis->setObjectName(QStringLiteral("helperText"));
    notaSintaxis->setWordWrap(true);
    auto* accionesEntrada = new QHBoxLayout();
    auto* botonEjemplo = new QPushButton(QStringLiteral("Cargar ejemplo"), panelEntrada);
    botonEjemplo->setObjectName(QStringLiteral("exampleButton"));
    auto* botonAnalizar = new QPushButton(QStringLiteral("▶   Analizar"), panelEntrada);
    botonAnalizar->setObjectName(QStringLiteral("analyzeButton"));
    botonAnalizar->setMinimumHeight(42);
    accionesEntrada->addWidget(botonEjemplo);
    accionesEntrada->addWidget(botonAnalizar, 1);
    layoutEntrada->addLayout(tituloEntradaFila);
    layoutEntrada->addWidget(ayudaEntrada);
    layoutEntrada->addWidget(textoReglas, 1);
    layoutEntrada->addWidget(notaSintaxis);
    layoutEntrada->addLayout(accionesEntrada);

    auto* panelResultados = new QWidget(divisor);
    auto* layoutResultados = new QVBoxLayout(panelResultados);
    layoutResultados->setContentsMargins(0, 0, 0, 0);
    layoutResultados->setSpacing(12);

    auto* resumen = new QFrame(panelResultados);
    resumen->setObjectName(QStringLiteral("summaryPanel"));
    auto* layoutResumen = new QHBoxLayout(resumen);
    layoutResumen->setContentsMargins(15, 13, 15, 13);
    layoutResumen->setSpacing(10);

    auto* tarjetaEstado = new QFrame(resumen);
    tarjetaEstado->setObjectName(QStringLiteral("statusCard"));
    auto* layoutEstado = new QVBoxLayout(tarjetaEstado);
    layoutEstado->setContentsMargins(13, 9, 13, 9);
    layoutEstado->setSpacing(3);
    auto* encabezadoEstado = new QLabel(QStringLiteral("ESTADO DEL ANÁLISIS"), tarjetaEstado);
    encabezadoEstado->setObjectName(QStringLiteral("statusEyebrow"));
    etiquetaEstado = new QLabel(QStringLiteral("Listo para analizar"), tarjetaEstado);
    etiquetaEstado->setObjectName(QStringLiteral("resultStatus"));
    layoutEstado->addWidget(encabezadoEstado);
    layoutEstado->addWidget(etiquetaEstado);

    auto crearTarjetaConteo = [resumen](const QString& objeto, const QString& colorConteo,
                                        const QString& tituloConteo, QLabel*& salida) {
        auto* tarjeta = new QFrame(resumen);
        tarjeta->setObjectName(objeto);
        auto* layout = new QVBoxLayout(tarjeta);
        layout->setContentsMargins(14, 8, 14, 8);
        layout->setSpacing(0);
        salida = new QLabel(QStringLiteral("0"), tarjeta);
        salida->setObjectName(colorConteo);
        auto* leyenda = new QLabel(tituloConteo, tarjeta);
        leyenda->setObjectName(QStringLiteral("countCaption"));
        layout->addWidget(salida);
        layout->addWidget(leyenda);
        return tarjeta;
    };
    auto* tarjetaAbiertas = crearTarjetaConteo(QStringLiteral("openCard"), QStringLiteral("openCount"),
                                                QStringLiteral("RAMAS ABIERTAS"), etiquetaRamasAbiertas);
    auto* tarjetaCerradas = crearTarjetaConteo(QStringLiteral("closedCard"), QStringLiteral("closedCount"),
                                                QStringLiteral("RAMAS CERRADAS"), etiquetaRamasCerradas);
    layoutResumen->addWidget(tarjetaEstado, 1);
    layoutResumen->addWidget(tarjetaAbiertas);
    layoutResumen->addWidget(tarjetaCerradas);

    pestanas = new QTabWidget(panelResultados);
    pestanas->setDocumentMode(true);
    pestanas->setMovable(false);
    tablaAtomos = new QTableWidget(pestanas);
    tablaAtomos->setColumnCount(2);
    tablaAtomos->setHorizontalHeaderLabels({QStringLiteral("Símbolo"), QStringLiteral("Proposición")});
    tablaAtomos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaAtomos->verticalHeader()->setVisible(false);
    tablaAtomos->setAlternatingRowColors(true);
    tablaAtomos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaAtomos->setShowGrid(false);

    tablaFormulas = new QTableWidget(pestanas);
    tablaFormulas->setColumnCount(2);
    tablaFormulas->setHorizontalHeaderLabels({QStringLiteral("Regla original"), QStringLiteral("Fórmula")});
    tablaFormulas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaFormulas->verticalHeader()->setVisible(false);
    tablaFormulas->setAlternatingRowColors(true);
    tablaFormulas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaFormulas->setShowGrid(false);

    lienzo = new LienzoArbol(pestanas);
    tablaDetalle = new QTableWidget(pestanas);
    tablaDetalle->setObjectName(QStringLiteral("detailTable"));
    tablaDetalle->setColumnCount(5);
    tablaDetalle->setHorizontalHeaderLabels({
        QStringLiteral("Rama"), QStringLiteral("Estado"), QStringLiteral("Recorrido"),
        QStringLiteral("Contradicción"), QStringLiteral("Modelo")});
    tablaDetalle->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tablaDetalle->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tablaDetalle->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tablaDetalle->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tablaDetalle->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    tablaDetalle->verticalHeader()->setVisible(false);
    tablaDetalle->setAlternatingRowColors(true);
    tablaDetalle->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaDetalle->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaDetalle->setShowGrid(false);
    tablaDetalle->setWordWrap(true);
    textoResultados = new QTextEdit(pestanas);
    textoResultados->setObjectName(QStringLiteral("resultsView"));
    textoResultados->setReadOnly(true);

    pestanas->addTab(textoResultados, QStringLiteral("✦   Resumen"));
    pestanas->addTab(lienzo, QStringLiteral("⌘   Tableau"));
    pestanas->addTab(tablaFormulas, QStringLiteral("ƒ   Fórmulas"));
    pestanas->addTab(tablaAtomos, QStringLiteral("◉   Átomos"));
    pestanas->addTab(tablaDetalle, QStringLiteral("☷   Ramas"));

    layoutResultados->addWidget(resumen);
    layoutResultados->addWidget(pestanas, 1);
    divisor->addWidget(panelEntrada);
    divisor->addWidget(panelResultados);
    divisor->setStretchFactor(0, 0);
    divisor->setStretchFactor(1, 1);
    divisor->setSizes({390, 900});
    principal->addWidget(divisor, 1);
    setCentralWidget(central);

    connect(botonAnalizar, &QPushButton::clicked, this, &VentanaPrincipal::analizar);
    connect(botonEjemplo, &QPushButton::clicked, this, &VentanaPrincipal::cargarEjemplo);
    statusBar()->showMessage(QStringLiteral("  Listo  ·  Ingresa las reglas y analiza la base de conocimiento."));
}

void VentanaPrincipal::cargarEjemplo() {
    textoReglas->setPlainText(
        QStringLiteral("si llueve entonces suelo mojado\nllueve\nno suelo mojado"));
}

void VentanaPrincipal::limpiarVistas() {
    tablaAtomos->setRowCount(0);
    tablaFormulas->setRowCount(0);
    lienzo->limpiar();
    tablaDetalle->setRowCount(0);
    textoResultados->clear();
    etiquetaEstado->setText(QStringLiteral("Listo para analizar"));
    etiquetaEstado->setStyleSheet(QString());
    etiquetaRamasAbiertas->setText(QStringLiteral("0"));
    etiquetaRamasCerradas->setText(QStringLiteral("0"));
}

void VentanaPrincipal::mostrarAtomos(const QVector<Atomo>& atomos) {
    tablaAtomos->setRowCount(atomos.size());
    for (int i = 0; i < atomos.size(); ++i) {
        tablaAtomos->setItem(i, 0, new QTableWidgetItem(atomos[i].letra));
        tablaAtomos->setItem(i, 1, new QTableWidgetItem(atomos[i].significado));
    }
}

void VentanaPrincipal::mostrarFormulas(const QVector<FormulaRegla>& reglas) {
    tablaFormulas->setRowCount(reglas.size());
    for (int i = 0; i < reglas.size(); ++i) {
        tablaFormulas->setItem(i, 0, new QTableWidgetItem(reglas[i].textoOriginal));
        tablaFormulas->setItem(i, 1, new QTableWidgetItem(reglas[i].formula->toString()));
    }
}

void VentanaPrincipal::mostrarResultados(const ResultadoMotor& resultado, const RegistroAtomos& registro) {
    etiquetaRamasAbiertas->setText(QString::number(resultado.ramasAbiertas));
    etiquetaRamasCerradas->setText(QString::number(resultado.ramasCerradas));
    QStringList lineas;
    if (resultado.ramasAbiertas > 0) {
        etiquetaEstado->setText(QStringLiteral("BASE CONSISTENTE"));
        etiquetaEstado->setStyleSheet(QStringLiteral("color: #53d3a4;"));
        lineas << QStringLiteral("La base de conocimiento es CONSISTENTE.")
               << QStringLiteral("Ramas abiertas: %1").arg(resultado.ramasAbiertas)
               << QStringLiteral("Ramas cerradas: %1").arg(resultado.ramasCerradas)
               << QString();

        int numeroModelo = 1;
        std::function<void(const NodoPtr&)> agregarModelos = [&](const NodoPtr& nodo) {
            if (!nodo) return;
            if (nodo->esHoja()) {
                if (!nodo->abierta) return;
                QMap<QString, bool> valores;
                for (const EntradaFormula& entrada : nodo->entradas) {
                    if (!entrada.formula) continue;
                    if (entrada.formula->tipo == TipoFormula::Atomo) {
                        valores.insert(entrada.formula->atomoClave, true);
                    } else if (entrada.formula->esNegacionDeAtomo()) {
                        valores.insert(entrada.formula->izquierda->atomoClave, false);
                    }
                }
                QStringList asignaciones;
                for (const Atomo& atomo : registro.atomos()) {
                    const QString clave = atomo.clave;
                    asignaciones.append(QStringLiteral("%1=%2").arg(
                        atomo.letra,
                        valores.contains(clave)
                            ? (valores.value(clave) ? QStringLiteral("verdadero") : QStringLiteral("falso"))
                            : QStringLiteral("libre")));
                }
                lineas.append(QStringLiteral("Modelo %1: {%2}")
                                  .arg(numeroModelo++)
                                  .arg(asignaciones.join(QStringLiteral(", "))));
                return;
            }
            for (const NodoPtr& hijo : nodo->hijos) agregarModelos(hijo);
        };
        agregarModelos(resultado.raiz);
    } else {
        etiquetaEstado->setText(QStringLiteral("BASE INCONSISTENTE"));
        etiquetaEstado->setStyleSheet(QStringLiteral("color: #77aaff;"));
        lineas << QStringLiteral("La base de conocimiento es INCONSISTENTE.")
               << QStringLiteral("Todas las ramas se cerraron.")
               << QStringLiteral("Ramas cerradas: %1").arg(resultado.ramasCerradas);
    }
    textoResultados->setPlainText(lineas.join(QLatin1Char('\n')));
}

void VentanaPrincipal::mostrarDetalle(const NodoPtr& raiz) {
    tablaDetalle->setRowCount(0);
    if (!raiz) return;

    int numeroRama = 0;
    QStringList recorrido;
    std::function<void(const NodoPtr&)> agregarRama = [&](const NodoPtr& nodo) {
        const size_t heredadas = nodo->padre
            ? static_cast<size_t>(nodo->padre->entradas.size())
            : 0;
        QStringList nuevas;
        for (size_t i = heredadas; i < nodo->entradas.size(); ++i) {
            const EntradaFormula& entrada = nodo->entradas[i];
            if (entrada.formula) nuevas.append(entrada.formula->toString());
        }
        if (!nuevas.isEmpty()) {
            recorrido.append(QStringLiteral("Paso %1: %2")
                                 .arg(nodo->id)
                                 .arg(nuevas.join(QStringLiteral(", "))));
        }

        if (nodo->esHoja()) {
            const int fila = tablaDetalle->rowCount();
            tablaDetalle->insertRow(fila);
            const QString estado = nodo->cerrada ? QStringLiteral("CERRADA")
                : nodo->abierta ? QStringLiteral("ABIERTA")
                                : QStringLiteral("PENDIENTE");
            QStringList literales;
            QMap<QString, QString> valores;
            for (const EntradaFormula& entrada : nodo->entradas) {
                if (!entrada.formula) continue;
                if (entrada.formula->tipo == TipoFormula::Atomo) {
                    valores.insert(entrada.formula->atomoClave,
                                   entrada.formula->atomoLetra.isEmpty()
                                       ? entrada.formula->atomoTexto
                                       : entrada.formula->atomoLetra);
                } else if (entrada.formula->esNegacionDeAtomo()) {
                    const FormulaPtr& atomo = entrada.formula->izquierda;
                    valores.insert(atomo->atomoClave,
                                   QStringLiteral("¬") + (atomo->atomoLetra.isEmpty()
                                       ? atomo->atomoTexto : atomo->atomoLetra));
                }
            }
            for (auto valor = valores.cbegin(); valor != valores.cend(); ++valor) {
                literales.append(valor.value());
            }
            const QString modelo = nodo->abierta
                ? (literales.isEmpty() ? QStringLiteral("Sin restricciones")
                                       : literales.join(QStringLiteral(", ")))
                : QStringLiteral("—");

            const QStringList celdas{
                QStringLiteral("Rama %1").arg(++numeroRama),
                estado,
                recorrido.join(QStringLiteral("\n")),
                nodo->contradiccion.isEmpty() ? QStringLiteral("—") : nodo->contradiccion,
                modelo
            };
            const QColor color = nodo->cerrada ? QColor(50, 106, 192)
                : nodo->abierta ? QColor(25, 132, 96) : QColor(89, 99, 122);
            for (int columna = 0; columna < celdas.size(); ++columna) {
                auto* item = new QTableWidgetItem(celdas[columna]);
                item->setForeground(color);
                if (columna == 0 || columna == 1) {
                    QFont fuente = item->font();
                    fuente.setBold(true);
                    item->setFont(fuente);
                }
                tablaDetalle->setItem(fila, columna, item);
            }
            tablaDetalle->setRowHeight(fila, qMax(58, recorrido.size() * 24 + 24));
        } else {
            for (const NodoPtr& hijo : nodo->hijos) agregarRama(hijo);
        }

        if (!nuevas.isEmpty()) recorrido.removeLast();
    };
    agregarRama(raiz);
}

void VentanaPrincipal::analizar() {
    limpiarVistas();

    AnalizadorSemantico analizador;
    QVector<FormulaRegla> reglas;
    QString error;
    if (!analizador.analizarBase(textoReglas->toPlainText(), reglas, error)) {
        QMessageBox::warning(this, QStringLiteral("No se pudo analizar"), error);
        statusBar()->showMessage(QStringLiteral("Corrige la base de conocimiento para continuar."));
        return;
    }

    RegistroAtomos registro;
    registro.limpiar();
    QVector<FormulaPtr> formulas;
    formulas.reserve(reglas.size());
    for (const FormulaRegla& regla : reglas) {
        registro.registrarFormula(regla.formula);
        formulas.append(regla.formula);
    }

    MotorArbol motor;
    const ResultadoMotor resultado = motor.construir(formulas);
    mostrarAtomos(registro.atomos());
    mostrarFormulas(reglas);
    lienzo->mostrarArbol(resultado.raiz);
    mostrarDetalle(resultado.raiz);
    mostrarResultados(resultado, registro);
    pestanas->setCurrentWidget(lienzo);
    statusBar()->showMessage(QStringLiteral("Análisis terminado."));
}
