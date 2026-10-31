#pragma once
#include <QMainWindow>
#include <QTextEdit>
#include <QTabWidget>
#include <QTableWidget>
#include <QLabel>
#include "LienzoArbol.hpp"
#include "AnalizadorSemantico.hpp"
#include "RegistroAtomos.hpp"
#include "MotorArbol.hpp"

class VentanaPrincipal : public QMainWindow {
public:
  VentanaPrincipal(QWidget* parent=nullptr);
private:
  void analizar();
  void configurarUi();
  void cargarEjemplo();
  void limpiarVistas();
  void mostrarAtomos(const QVector<Atomo>& atomos);
  void mostrarFormulas(const QVector<FormulaRegla>& reglas);
  void mostrarResultados(const ResultadoMotor& res, const RegistroAtomos& reg);
  void mostrarDetalle(const NodoPtr& raiz);
  QLabel* etiquetaEstado;
  QLabel* etiquetaRamasAbiertas;
  QLabel* etiquetaRamasCerradas;
  QTextEdit* textoReglas;
  QTabWidget* pestanas;
  QTableWidget* tablaAtomos;
  QTableWidget* tablaFormulas;
  LienzoArbol* lienzo;
  QTableWidget* tablaDetalle;
  QTextEdit* textoResultados;
};