#!/usr/bin/python
# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui
from csv import writer
from ConfigParser import ConfigParser

from freesteam import *
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg, NavigationToolbar2QT
from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D
from numpy import meshgrid, zeros, arange, linspace, concatenate, max, min, transpose, logspace, log, arctan, pi

from images import images_rc
from units import unidades, pressure, temperature, enthalpy, entropy, specific_volume, config


class Plot(FigureCanvasQTAgg):
    def __init__(self, parent=None, width=5, height=4, dpi=100, dim=3):
        self.fig = pyplot.Figure(figsize=(width, height), dpi=dpi)
        FigureCanvasQTAgg.__init__(self, self.fig)
        self.setParent(parent)
        self.dim=dim
        
        if dim==2:
            self.axes2D = self.fig.add_subplot(111)
            self.axes2D.figure.subplots_adjust(left=0.09, right=0.98, bottom=0.08, top=0.98)
        else:
            self.axes3D = Axes3D(self.fig)
        
#        self.axes3D.hold(True)
#        self.axes3D.set_axisbelow(True)
        
        FigureCanvasQTAgg.setSizePolicy(self, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        FigureCanvasQTAgg.updateGeometry(self)


    def plot_sat(self, xsat, ysat, zsat=0):
        """Método que dibuja la línea de saturación"""
        if self.dim==3:
            self.satliq=self.axes3D.plot3D(xsat[0], ysat[0], zsat[0],'k-')
            self.satgas=self.axes3D.plot3D(xsat[1], ysat[1], zsat[1],'k-')
        else:
            self.satliq=self.axes2D.plot(xsat[0], ysat[0],'k-')
            self.satgas=self.axes2D.plot(xsat[1], ysat[1],'k-')


    def plot_3D(self, labels, xdata, ydata, zdata):
        """Método que dibuja la matriz de datos"""
        self.axes3D.clear()
        self.axes3D.plot_wireframe(xdata, ydata, zdata, rstride=1, cstride=1)
        self.axes3D.set_xlabel(labels[0])
        self.axes3D.set_ylabel(labels[1])
        self.axes3D.set_zlabel(labels[2])
        self.axes3D.mouse_init()
        
    def plot_2D(self, labels, bool):
        self.axes2D.clear()
        self.axes2D.grid(bool)        
        self.axes2D.axes.set_xlabel(labels[0], size='12')
        self.axes2D.axes.set_ylabel(labels[1], size='12')
        
    def plot_labels(self, tipo, x, y, label, angle=0):
        linea=[]
        for i in range(len(label)):
            linea.append(self.axes2D.axes.annotate(label[i], (x[i], y[i]), rotation=angle[i], size="xx-small", horizontalalignment="center", verticalalignment="center"))
        if tipo =="T":
            self.Isoterma_label=linea
        elif tipo =="P":
            self.Isobara_label=linea
        elif tipo =="V":
            self.Isocora_label=linea
        elif tipo =="S":
            self.Isoentropica_label=linea
        elif tipo =="H":
            self.Isoentalpica_label=linea
        elif tipo =="X":
            self.IsoX_label=linea



    def plot_puntos(self, x, y, z=0):
        """Método que dibuja puntos individuales"""
        colores=config.colors(len(x))
        self.puntos=[]
        if self.dim==3:
            for i in range(len(x)):
                self.puntos.append(self.axes3D.plot3D([x[i]], [y[i]], [z[i]], color=colores[i], marker="o"))
        else:
            for i in range(len(x)):
                self.puntos.append(self.axes2D.plot([x[i]], [y[i]], color=colores[i], marker="o"))
            
            
    def plot_Isolinea(self, tipo, x, y, z=0, color="#000000", grosor=1, estilo=0):
        """Método que dibuja las isolineas"""
        if estilo==0:
            linestyle='-'
        elif estilo==1:
            linestyle='--'
        elif estilo==2:
            linestyle='-.'
        elif estilo==3:
            linestyle=':'
            
        linea=[]
        if self.dim==3:
            for i in range(len(x)):
                linea.append(self.axes3D.plot3D(x[i], y[i], z[i], color, lw=grosor, ls=linestyle))
        else:
            for i in range(len(x)):
                linea.append(self.axes2D.plot(x[i], y[i], color, lw=grosor, ls=linestyle))

        if tipo =="T":
            self.Isoterma=linea
        elif tipo =="P":
            self.Isobara=linea
        elif tipo =="V":
            self.Isocora=linea
        elif tipo =="S":
            self.Isoentropica=linea
        elif tipo =="H":
            self.Isoentalpica=linea
        elif tipo =="X":
            self.IsoX=linea


class Ventana_Lista_Puntos(QtGui.QDialog):
    """Dialogo que muestra la lísta de puntos especificados por el usuario así como sus propiedades"""
    def setupUi(self, Dialog, puntos):
        self.puntos=puntos
        self.gridLayout = QtGui.QGridLayout(Dialog)
        self.listWidget = QtGui.QListWidget(Dialog)
        self.gridLayout.addWidget(self.listWidget, 0, 0, 3, 1)
        self.botonBorrar = QtGui.QPushButton(Dialog)
        self.gridLayout.addWidget(self.botonBorrar, 0, 1, 1, 1)
        self.botonPropiedades = QtGui.QPushButton(Dialog)
        self.botonPropiedades.setCheckable(True)
        self.gridLayout.addWidget(self.botonPropiedades, 1, 1, 1, 1)
        self.buttonBox = QtGui.QDialogButtonBox(Dialog)
        self.buttonBox.setOrientation(QtCore.Qt.Vertical)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Ok)
        self.gridLayout.addWidget(self.buttonBox, 2, 1, 1, 1)
        self.tablaPropiedades=QtGui.QTableWidget(Dialog)
        self.tablaPropiedades.setVisible(False)
        self.tablaPropiedades.setRowCount(16)
        self.gridLayout.addWidget(self.tablaPropiedades, 3, 0, 1, 2)
        Dialog.setFixedSize(500, Dialog.minimumHeight())
        
        self.colores=config.colors(len(self.puntos))
        
        self.rellenarLista()
        self.rellenarTabla()
        self.retranslateUi(Dialog)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("accepted()"), Dialog.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("rejected()"), Dialog.reject)
        QtCore.QObject.connect(self.botonBorrar, QtCore.SIGNAL("clicked()"), self.on_botonBorrar_clicked)
        QtCore.QObject.connect(self.botonPropiedades, QtCore.SIGNAL("toggled(bool)"), self.tablaPropiedades.setVisible)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(QtGui.QApplication.translate("Dialog", "Puntos individuales", None, QtGui.QApplication.UnicodeUTF8))
        self.botonBorrar.setText(QtGui.QApplication.translate("Dialog", "Borrar", None, QtGui.QApplication.UnicodeUTF8))
        self.botonPropiedades.setText(QtGui.QApplication.translate("Dialog", "Propiedades", None, QtGui.QApplication.UnicodeUTF8))
        self.tablaPropiedades.setVerticalHeaderItem(0, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Presión", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Pressure").text()))
        self.tablaPropiedades.setVerticalHeaderItem(1, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Temperatura", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Temperature").text()))
        self.tablaPropiedades.setVerticalHeaderItem(2, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Volumen", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("SpecificVolume").text()))
        self.tablaPropiedades.setVerticalHeaderItem(3, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Entalpía", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Enthalpy").text()))
        self.tablaPropiedades.setVerticalHeaderItem(4, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Entropía", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("SpecificHeat", "Entropy").text()))
        self.tablaPropiedades.setVerticalHeaderItem(5, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", u"Fracción de vapor", None, QtGui.QApplication.UnicodeUTF8)))
        self.tablaPropiedades.setVerticalHeaderItem(6, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Energía interna", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Enthalpy").text()))
        self.tablaPropiedades.setVerticalHeaderItem(7, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Energía de Gibbs", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Enthalpy").text()))
        self.tablaPropiedades.setVerticalHeaderItem(8, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Energía de Helmholtz", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Enthalpy").text()))
        self.tablaPropiedades.setVerticalHeaderItem(9, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Densidad", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Density").text()))
        self.tablaPropiedades.setVerticalHeaderItem(10, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Cp", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("SpecificHeat").text()))
        self.tablaPropiedades.setVerticalHeaderItem(11, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Cv", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("SpecificHeat").text()))
        self.tablaPropiedades.setVerticalHeaderItem(12, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Conductividad", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("ThermalConductivity").text()))
        self.tablaPropiedades.setVerticalHeaderItem(13, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Viscosidad", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Viscosity").text()))
        self.tablaPropiedades.setVerticalHeaderItem(14, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Velocidad del sonido", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Speed").text()))
        self.tablaPropiedades.setVerticalHeaderItem(15, QtGui.QTableWidgetItem(QtGui.QApplication.translate("Dialog", "Región", None, QtGui.QApplication.UnicodeUTF8)))

    def rellenarLista(self):
        #TODO: Mejorar el método de iconizar los puntos mediante otro método que no use svg sino codigos de colores simplemente
        self.listWidget.clear()
        for i, punto in enumerate(self.puntos):
            self.listWidget.addItem(str(i+1)+" - "+config.representacion(unidades.Temperature(punto.T).config)+" "+config.Configuracion("Temperature").text()+", "+config.representacion(unidades.Pressure(punto.p).config)+" "+config.Configuracion("Pressure").text()+", x="+config.representacion(punto.x))
            pixmap=QtGui.QPixmap(10, 10)
            pixmap.fill(QtGui.QColor(self.colores[i]))
            self.listWidget.item(i).setIcon(QtGui.QIcon(pixmap))
            
    def rellenarTabla(self):
        if len(self.puntos)==0:
            self.tablaPropiedades.setFixedHeight(404)
        elif len(self.puntos)<5:
            self.tablaPropiedades.setFixedHeight(428)
        else:
            self.tablaPropiedades.setFixedHeight(444)
        self.tablaPropiedades.setColumnCount(len(self.puntos))
        for i in range(16):
            self.tablaPropiedades.setRowHeight(i,25)
        for i, punto in enumerate(self.puntos):
            pixmap=QtGui.QPixmap(10, 10)
            pixmap.fill(QtGui.QColor(self.colores[i]))
            self.tablaPropiedades.setHorizontalHeaderItem(i, QtGui.QTableWidgetItem(QtGui.QIcon(pixmap), str(i+1)))
            self.tablaPropiedades.setItem(0, i, QtGui.QTableWidgetItem(config.representacion(unidades.Pressure(punto.p).config)))
            self.tablaPropiedades.setItem(1, i, QtGui.QTableWidgetItem(config.representacion(unidades.Temperature(punto.T).config)))
            self.tablaPropiedades.setItem(2, i, QtGui.QTableWidgetItem(config.representacion(unidades.SpecificVolume(punto.v).config)))
            self.tablaPropiedades.setItem(3, i, QtGui.QTableWidgetItem(config.representacion(unidades.Enthalpy(punto.h).config)))
            self.tablaPropiedades.setItem(4, i, QtGui.QTableWidgetItem(config.representacion(unidades.SpecificHeat(punto.s).config("Entropy"))))
            self.tablaPropiedades.setItem(5, i, QtGui.QTableWidgetItem(config.representacion(punto.x)))
            self.tablaPropiedades.setItem(6, i, QtGui.QTableWidgetItem(config.representacion(unidades.Enthalpy(punto.u).config)))
            self.tablaPropiedades.setItem(7, i, QtGui.QTableWidgetItem(config.representacion(unidades.Enthalpy(punto.s).config)))
            self.tablaPropiedades.setItem(8, i, QtGui.QTableWidgetItem(config.representacion(unidades.Enthalpy(punto.s).config)))
            self.tablaPropiedades.setItem(9, i, QtGui.QTableWidgetItem(config.representacion(unidades.Density(punto.rho).config)))
            self.tablaPropiedades.setItem(10, i, QtGui.QTableWidgetItem(config.representacion(unidades.SpecificHeat(punto.cp).config())))
            self.tablaPropiedades.setItem(11, i, QtGui.QTableWidgetItem(config.representacion(unidades.SpecificHeat(punto.cv).config())))
            self.tablaPropiedades.setItem(12, i, QtGui.QTableWidgetItem(config.representacion(unidades.ThermalConductivity(punto.k).config)))
            self.tablaPropiedades.setItem(13, i, QtGui.QTableWidgetItem(config.representacion(unidades.Viscosity(punto.mu).config)))
            if punto.region !='\x04' and  punto.region !='\x03':
                self.tablaPropiedades.setItem(14, i, QtGui.QTableWidgetItem(config.representacion(unidades.Speed(punto.w).config)))
            else:
                self.tablaPropiedades.setItem(14, i, QtGui.QTableWidgetItem('nan'))
            if punto.region =='\x01':
                self.tablaPropiedades.setItem(15, i, QtGui.QTableWidgetItem('1'))
            elif punto.region =='\x02':
                self.tablaPropiedades.setItem(15, i, QtGui.QTableWidgetItem('2'))
            elif punto.region =='\x03':
                self.tablaPropiedades.setItem(15, i, QtGui.QTableWidgetItem('3'))
            elif punto.region =='\x04':
                self.tablaPropiedades.setItem(15, i, QtGui.QTableWidgetItem('4'))
            elif punto.region =='\x05':
                self.tablaPropiedades.setItem(15, i, QtGui.QTableWidgetItem('5'))
            for j in range(16):
                self.tablaPropiedades.item(j, i).setTextAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
                
        self.tablaPropiedades.resizeColumnsToContents()

    def on_botonBorrar_clicked(self):
        """Borra el punto seleccionado de la lista"""
        if self.listWidget.currentRow()>=0:
            self.puntos.pop(self.listWidget.currentRow())
        self.rellenarLista()
        self.rellenarTabla()


class Ui_SteamTables(QtGui.QMainWindow):
    def setupUi(self, SteamTables):
        SteamTables.resize(1024,800)
        self.semaforo=QtCore.QSemaphore(1)
        self.centralwidget = QtGui.QWidget(SteamTables)
        SteamTables.setCentralWidget(self.centralwidget)
        
        #menus
        self.menubar = QtGui.QMenuBar(SteamTables)
        self.menubar.setGeometry(QtCore.QRect(0,0,700,30))
        self.menuArchivo = QtGui.QMenu(self.menubar)
        self.menuGrafico = QtGui.QMenu(self.menubar)
        self.menuAyuda = QtGui.QMenu(self.menubar)
        SteamTables.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(SteamTables)
        SteamTables.setStatusBar(self.statusbar)
        self.progresbar=QtGui.QProgressBar(SteamTables)
        self.progresbar.setVisible(False)
        self.progresbar.setFixedSize(80, 15)
        self.statusbar.addPermanentWidget(self.progresbar)
        self.actionCSV = QtGui.QAction(SteamTables)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/button/filesave.png"),QtGui.QIcon.Normal,QtGui.QIcon.Off)
        self.actionCSV.setIcon(icon)
        self.actionCSV.setShortcut("Ctrl+E")
        self.actionCSV.setEnabled(False)
        self.actionSalir = QtGui.QAction(SteamTables)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/button/exit.png"),QtGui.QIcon.Normal,QtGui.QIcon.Off)
        self.actionSalir.setIcon(icon)
        self.actionSalir.setShortcut("Alt+F4")
        self.actionTipoGrafico=QtGui.QActionGroup(SteamTables)
        self.action2D = QtGui.QAction(self.actionTipoGrafico)
        self.action2D.setCheckable(True)
        self.action2D.setShortcut("Ctrl+2")
        self.action3D = QtGui.QAction(self.actionTipoGrafico)
        self.action3D.setCheckable(True)
        self.action3D.setChecked(True)
        self.action3D.setShortcut("Ctrl+3")
        self.actionMostrarBarra = QtGui.QAction(SteamTables)
        self.actionMostrarBarra.setCheckable(True)
        self.actionMostrarBarra.setChecked(False)
        self.actionMostrarBarra.setShortcut("Ctrl+T")
        self.actionDibujarSaturacion = QtGui.QAction(SteamTables)
        self.actionDibujarSaturacion.setCheckable(True)
        self.actionDibujarSaturacion.setChecked(True)
        self.actionDibujarSaturacion.setShortcut("Ctrl+S")
        self.actionMostrarPuntos = QtGui.QAction(SteamTables)
        self.actionMostrarPuntos.setCheckable(True)
        self.actionMostrarPuntos.setChecked(True)
        self.actionMostrarPuntos.setShortcut("Ctrl+P")
        self.actionAcerca_de = QtGui.QAction(SteamTables)

        self.actionAcerca_deQt= QtGui.QAction(SteamTables)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/button/AboutQt.png"),QtGui.QIcon.Normal,QtGui.QIcon.Off)
        self.actionAcerca_deQt.setIcon(icon)
        self.menuArchivo.addAction(self.actionCSV)
        self.menuArchivo.addAction(self.actionSalir)
        self.menuGrafico.addAction(self.action2D)
        self.menuGrafico.addAction(self.action3D)
        self.menuGrafico.addSeparator()
        self.menuGrafico.addAction(self.actionMostrarBarra)
        self.menuGrafico.addAction(self.actionDibujarSaturacion)
        self.menuGrafico.addAction(self.actionMostrarPuntos)
        self.menuAyuda.addAction(self.actionAcerca_de)
        self.menuAyuda.addAction(self.actionAcerca_deQt)
        self.menubar.addAction(self.menuArchivo.menuAction())
        self.menubar.addAction(self.menuGrafico.menuAction())
        self.menubar.addAction(self.menuAyuda.menuAction())
                
        #Ventana principal
        self.gridLayout = QtGui.QGridLayout(self.centralwidget)
        self.toolBox = QtGui.QToolBox(self.centralwidget)
        self.page_1 = QtGui.QWidget()
        self.page_1.setGeometry(QtCore.QRect(0,0,302,390))
        self.gridLayout_2 = QtGui.QGridLayout(self.page_1)
        self.tabla = QtGui.QTableWidget(self.page_1)
        self.gridLayout_2.addWidget(self.tabla,0, 0, 1, 1)
        self.toolBox.addItem(self.page_1,"")
        self.page_Plot = QtGui.QWidget()
        self.page_Plot.setGeometry(QtCore.QRect(0,0,274,406))
        self.gridLayout_3 = QtGui.QGridLayout(self.page_Plot)
        self.checkIsoTherm=QtGui.QCheckBox(self.page_Plot)
        self.gridLayout_3.addWidget(self.checkIsoTherm,0,0,1,1)
        self.checkIsoBar=QtGui.QCheckBox(self.page_Plot)
        self.gridLayout_3.addWidget(self.checkIsoBar,0,1,1,1)
        self.checkIsoEnth=QtGui.QCheckBox(self.page_Plot)
        self.gridLayout_3.addWidget(self.checkIsoEnth,0,2,1,1)
        self.checkIsoEntr=QtGui.QCheckBox(self.page_Plot)
        self.gridLayout_3.addWidget(self.checkIsoEntr,0,3,1,1)
        self.checkIsoVol=QtGui.QCheckBox(self.page_Plot)
        self.gridLayout_3.addWidget(self.checkIsoVol,0,4,1,1)
        self.checkIsoX=QtGui.QCheckBox(self.page_Plot)
        self.gridLayout_3.addWidget(self.checkIsoX,0,5,1,1)
        self.diagrama2D = Plot(self.page_Plot, dpi=100, dim=2)
        self.gridLayout_3.addWidget(self.diagrama2D,1,0,1,6)
        self.diagrama3D = Plot(self.page_Plot, dpi=100, dim=3)
        self.gridLayout_3.addWidget(self.diagrama3D,2,0,1,6)
        self.toolbar2D=NavigationToolbar2QT(self.diagrama2D, self.diagrama2D)
        self.gridLayout_3.addWidget(self.toolbar2D,3,0,1,6)
        self.toolbar3D=NavigationToolbar2QT(self.diagrama3D, self.diagrama3D)
        self.gridLayout_3.addWidget(self.toolbar3D,4,0,1,6)
        self.toolBox.addItem(self.page_Plot,"")
        self.gridLayout.addWidget(self.toolBox,0,0,1,1)

        #Toolbox parámetros Tabla
        self.dockWidget_Tabla = QtGui.QDockWidget(self.centralwidget)
        self.dockWidget_Tabla.setFloating(False)
        self.dockWidgetContents = QtGui.QWidget()
        self.gridLayout_4 = QtGui.QGridLayout(self.dockWidgetContents)
        self.label = QtGui.QLabel(self.dockWidgetContents)
        self.gridLayout_4.addWidget(self.label,0,0,1,1)
        self.ejesTabla = QtGui.QComboBox(self.dockWidgetContents)
        self.ejesTabla.setMinimumSize(QtCore.QSize(100,0))
        self.ejesTabla.setMaximumSize(QtCore.QSize(100,16777215))
        self.gridLayout_4.addWidget(self.ejesTabla,0,1,1,2)
        self.label_14 = QtGui.QLabel(self.dockWidgetContents)
        self.gridLayout_4.addWidget(self.label_14,1,0,1,1)
        self.variableTabla = QtGui.QComboBox(self.dockWidgetContents)
        self.variableTabla.setMinimumSize(QtCore.QSize(150,0))
        self.variableTabla.setMaximumSize(QtCore.QSize(150,16777215))
        self.gridLayout_4.addWidget(self.variableTabla,1,1,1,2)
        self.gridLayout_4.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Minimum,QtGui.QSizePolicy.Minimum),2,0,4,1)
        self.label_26 = QtGui.QLabel(self.dockWidgetContents)
        self.gridLayout_4.addWidget(self.label_26,3,1,1,1)
        self.label_26.setAlignment(QtCore.Qt.AlignCenter)
        self.label_27 = QtGui.QLabel(self.dockWidgetContents)
        self.gridLayout_4.addWidget(self.label_27,3,2,1,1)
        self.label_27.setAlignment(QtCore.Qt.AlignCenter)
        self.label_17 = QtGui.QLabel(self.dockWidgetContents)
        self.gridLayout_4.addWidget(self.label_17,6,0,1,1)
        self.abscisaInicio = QtGui.QLineEdit(self.dockWidgetContents)
        self.abscisaInicio.setMaximumSize(QtCore.QSize(80,16777215))
        self.abscisaInicio.setAlignment(QtCore.Qt.AlignRight)        
        self.gridLayout_4.addWidget(self.abscisaInicio,4,1,1,1)
        self.ordenadaInicio = QtGui.QLineEdit(self.dockWidgetContents)
        self.ordenadaInicio.setMaximumSize(QtCore.QSize(80,16777215))
        self.ordenadaInicio.setAlignment(QtCore.Qt.AlignRight)        
        self.gridLayout_4.addWidget(self.ordenadaInicio,4,2,1,1)
        self.label_15 = QtGui.QLabel(self.dockWidgetContents)
        self.gridLayout_4.addWidget(self.label_15,4,0,1,1)
        self.label_16 = QtGui.QLabel(self.dockWidgetContents)
        self.gridLayout_4.addWidget(self.label_16,5,0,1,1)
        self.abscisaFin = QtGui.QLineEdit(self.dockWidgetContents)
        self.abscisaFin.setMaximumSize(QtCore.QSize(80,16777215))
        self.abscisaFin.setAlignment(QtCore.Qt.AlignRight)        
        self.gridLayout_4.addWidget(self.abscisaFin,5,1,1,1)
        self.ordenadaFin = QtGui.QLineEdit(self.dockWidgetContents)
        self.ordenadaFin.setMaximumSize(QtCore.QSize(80,16777215))
        self.ordenadaFin.setAlignment(QtCore.Qt.AlignRight)        
        self.gridLayout_4.addWidget(self.ordenadaFin,5,2,1,1)
        self.abscisaIntervalo = QtGui.QLineEdit(self.dockWidgetContents)
        self.abscisaIntervalo.setMaximumSize(QtCore.QSize(80,16777215))
        self.abscisaIntervalo.setAlignment(QtCore.Qt.AlignRight)        
        self.gridLayout_4.addWidget(self.abscisaIntervalo,6,1,1,1)
        self.ordenadaIntervalo = QtGui.QLineEdit(self.dockWidgetContents)
        self.ordenadaIntervalo.setMaximumSize(QtCore.QSize(80,16777215))
        self.ordenadaIntervalo.setAlignment(QtCore.Qt.AlignRight)        
        self.gridLayout_4.addWidget(self.ordenadaIntervalo,6,2,1,1)
        self.botonCalcular = QtGui.QPushButton(self.dockWidgetContents)
        self.botonCalcular.setLayoutDirection(QtCore.Qt.RightToLeft)
        self.botonCalcular.setMaximumSize(QtCore.QSize(80,16777215))
        self.gridLayout_4.addWidget(self.botonCalcular,7,0,1,3)
        self.dockWidget_Tabla.setWidget(self.dockWidgetContents)
        self.dockWidget_Tabla.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea)
        SteamTables.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget_Tabla)
        self.dockWidget_Tabla.setFeatures(QtGui.QDockWidget.DockWidgetMovable)

        #Toolbox configuración isolineas
        self.dockWidget_Isolineas = QtGui.QDockWidget(self.centralwidget)
        self.dockWidget_Isolineas.setFloating(False)
        self.dockWidgetContents_1 = QtGui.QWidget()
        self.gridLayout_7 = QtGui.QGridLayout(self.dockWidgetContents_1)
        self.Isolineas = QtGui.QTabWidget(self.dockWidgetContents_1)
        self.page = QtGui.QWidget()
        self.gridLayout = QtGui.QGridLayout(self.page)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.verticalLayout = QtGui.QVBoxLayout()
        self.label1 = QtGui.QLabel(self.page)
        self.label1.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout.addWidget(self.label1)
        self.T_ini = QtGui.QLineEdit(self.page)
        self.T_ini.setFixedSize(QtCore.QSize(80, 22))
        self.T_ini.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout.addWidget(self.T_ini)
        self.horizontalLayout.addLayout(self.verticalLayout)
        self.verticalLayout_2 = QtGui.QVBoxLayout()
        self.label2 = QtGui.QLabel(self.page)
        self.label2.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_2.addWidget(self.label2)
        self.T_fin = QtGui.QLineEdit(self.page)
        self.T_fin.setFixedSize(QtCore.QSize(80, 22))
        self.T_fin.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_2.addWidget(self.T_fin)
        self.horizontalLayout.addLayout(self.verticalLayout_2)
        self.verticalLayout_3 = QtGui.QVBoxLayout()
        self.label3 = QtGui.QLabel(self.page)
        self.label3.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_3.addWidget(self.label3)
        self.T_intervalo = QtGui.QLineEdit(self.page)
        self.T_intervalo.setFixedSize(QtCore.QSize(80, 22))
        self.T_intervalo.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_3.addWidget(self.T_intervalo)
        self.horizontalLayout.addLayout(self.verticalLayout_3)
        self.gridLayout.addLayout(self.horizontalLayout, 0, 0, 1, 3)
        self.PersonalizarIsoterma = QtGui.QCheckBox(self.page)
        self.gridLayout.addWidget(self.PersonalizarIsoterma, 2, 0, 1, 1)
        self.ListaIsoterma = QtGui.QLineEdit(self.page)
        self.gridLayout.addWidget(self.ListaIsoterma, 2, 1, 1, 2)
        self.IsotermaCritica = QtGui.QCheckBox(self.page)
        self.gridLayout.addWidget(self.IsotermaCritica, 3, 0, 1, 3)
        self.horizontalLayout_Isoterma= QtGui.QHBoxLayout()
        self.gridLayout.addLayout(self.horizontalLayout_Isoterma, 4, 0, 1, 3)
        self.label_31 = QtGui.QLabel(self.page)
        self.label_31.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.horizontalLayout_Isoterma.addWidget(self.label_31)
        self.IsotermaGrosor = QtGui.QDoubleSpinBox(self.page)
        self.IsotermaGrosor.setFixedWidth(50)
        self.IsotermaGrosor.setAlignment(QtCore.Qt.AlignRight)
        self.IsotermaGrosor.setRange(0.1, 5)
        self.IsotermaGrosor.setDecimals(1)
        self.IsotermaGrosor.setSingleStep(0.1)
        self.horizontalLayout_Isoterma.addWidget(self.IsotermaGrosor)
        self.IsotermaLinea = QtGui.QComboBox(self.page)
        self.IsotermaLinea.setFixedWidth(60)
        self.IsotermaLinea.setIconSize(QtCore.QSize(35, 18))
        self.horizontalLayout_Isoterma.addWidget(self.IsotermaLinea)
        self.IsotermaColorButton = QtGui.QToolButton(self.page)
        self.horizontalLayout_Isoterma.addWidget(self.IsotermaColorButton)
        self.Isolineas.addTab(self.page, "")
        self.page_2 = QtGui.QWidget()
        self.gridLayout_8 = QtGui.QGridLayout(self.page_2)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.verticalLayout_4 = QtGui.QVBoxLayout()
        self.label1_2 = QtGui.QLabel(self.page_2)
        self.label1_2.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_4.addWidget(self.label1_2)
        self.P_ini = QtGui.QLineEdit(self.page_2)
        self.P_ini.setFixedSize(QtCore.QSize(80, 22))
        self.P_ini.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_4.addWidget(self.P_ini)
        self.horizontalLayout_2.addLayout(self.verticalLayout_4)
        self.verticalLayout_5 = QtGui.QVBoxLayout()
        self.label2_2 = QtGui.QLabel(self.page_2)
        self.label2_2.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_5.addWidget(self.label2_2)
        self.P_fin = QtGui.QLineEdit(self.page_2)
        self.P_fin.setFixedSize(QtCore.QSize(80, 22))
        self.P_fin.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_5.addWidget(self.P_fin)
        self.horizontalLayout_2.addLayout(self.verticalLayout_5)
        self.verticalLayout_6 = QtGui.QVBoxLayout()
        self.label3_2 = QtGui.QLabel(self.page_2)
        self.label3_2.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_6.addWidget(self.label3_2)
        self.P_intervalo = QtGui.QLineEdit(self.page_2)
        self.P_intervalo.setFixedSize(QtCore.QSize(80, 22))
        self.P_intervalo.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_6.addWidget(self.P_intervalo)
        self.horizontalLayout_2.addLayout(self.verticalLayout_6)
        self.gridLayout_8.addLayout(self.horizontalLayout_2, 0, 0, 1, 3)
        self.PersonalizarIsobara = QtGui.QCheckBox(self.page_2)
        self.gridLayout_8.addWidget(self.PersonalizarIsobara, 2, 0, 1, 1)
        self.ListaIsobara = QtGui.QLineEdit(self.page_2)
        self.gridLayout_8.addWidget(self.ListaIsobara, 2, 1, 1, 2)
        self.IsobaraCritica = QtGui.QCheckBox(self.page_2)
        self.gridLayout_8.addWidget(self.IsobaraCritica, 3, 0, 1, 2)
        self.horizontalLayout_Isobara= QtGui.QHBoxLayout()
        self.gridLayout_8.addLayout(self.horizontalLayout_Isobara, 4, 0, 1, 3)
        self.label_32 = QtGui.QLabel(self.page_2)
        self.label_32.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.horizontalLayout_Isobara.addWidget(self.label_32)
        self.IsobaraGrosor = QtGui.QDoubleSpinBox(self.page_2)
        self.IsobaraGrosor.setFixedWidth(50)
        self.IsobaraGrosor.setAlignment(QtCore.Qt.AlignRight)
        self.IsobaraGrosor.setRange(0.1, 5)
        self.IsobaraGrosor.setDecimals(1)
        self.IsobaraGrosor.setSingleStep(0.1)
        self.horizontalLayout_Isobara.addWidget(self.IsobaraGrosor)
        self.IsobaraLinea = QtGui.QComboBox(self.page_2)
        self.IsobaraLinea.setFixedWidth(60)
        self.IsobaraLinea.setIconSize(QtCore.QSize(35, 18))
        self.horizontalLayout_Isobara.addWidget(self.IsobaraLinea)
        self.IsobaraColorButton = QtGui.QToolButton(self.page_2)
        self.horizontalLayout_Isobara.addWidget(self.IsobaraColorButton)
        self.Isolineas.addTab(self.page_2, "")
        self.page_3 = QtGui.QWidget()
        self.gridLayout_9 = QtGui.QGridLayout(self.page_3)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.verticalLayout_7 = QtGui.QVBoxLayout()
        self.label1_3 = QtGui.QLabel(self.page_3)
        self.label1_3.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_7.addWidget(self.label1_3)
        self.H_ini = QtGui.QLineEdit(self.page_3)
        self.H_ini.setFixedSize(QtCore.QSize(80, 22))
        self.H_ini.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_7.addWidget(self.H_ini)
        self.horizontalLayout_3.addLayout(self.verticalLayout_7)
        self.verticalLayout_8 = QtGui.QVBoxLayout()
        self.label2_3 = QtGui.QLabel(self.page_3)
        self.label2_3.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_8.addWidget(self.label2_3)
        self.H_fin = QtGui.QLineEdit(self.page_3)
        self.H_fin.setFixedSize(QtCore.QSize(80, 22))
        self.H_fin.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_8.addWidget(self.H_fin)
        self.horizontalLayout_3.addLayout(self.verticalLayout_8)
        self.verticalLayout_9 = QtGui.QVBoxLayout()
        self.label3_3 = QtGui.QLabel(self.page_3)
        self.label3_3.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_9.addWidget(self.label3_3)
        self.H_intervalo = QtGui.QLineEdit(self.page_3)
        self.H_intervalo.setFixedSize(QtCore.QSize(80, 22))
        self.H_intervalo.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_9.addWidget(self.H_intervalo)
        self.horizontalLayout_3.addLayout(self.verticalLayout_9)
        self.gridLayout_9.addLayout(self.horizontalLayout_3, 0, 0, 1, 3)
        self.PersonalizarIsoentalpica = QtGui.QCheckBox(self.page_3)
        self.gridLayout_9.addWidget(self.PersonalizarIsoentalpica, 2, 0, 1, 1)
        self.ListaIsoentalpica = QtGui.QLineEdit(self.page_3)
        self.gridLayout_9.addWidget(self.ListaIsoentalpica, 2, 1, 1, 2)
        self.IsoentalpicaCritica = QtGui.QCheckBox(self.page_3)
        self.gridLayout_9.addWidget(self.IsoentalpicaCritica, 3, 0, 1, 2)
        self.horizontalLayout_Isoentalpica= QtGui.QHBoxLayout()
        self.gridLayout_9.addLayout(self.horizontalLayout_Isoentalpica, 4, 0, 1, 3)
        self.label_33 = QtGui.QLabel(self.page_3)
        self.label_33.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.horizontalLayout_Isoentalpica.addWidget(self.label_33)
        self.IsoentalpicaGrosor = QtGui.QDoubleSpinBox(self.page_3)
        self.IsoentalpicaGrosor.setFixedWidth(50)
        self.IsoentalpicaGrosor.setAlignment(QtCore.Qt.AlignRight)
        self.IsoentalpicaGrosor.setRange(0.1, 5)
        self.IsoentalpicaGrosor.setDecimals(1)
        self.IsoentalpicaGrosor.setSingleStep(0.1)
        self.horizontalLayout_Isoentalpica.addWidget(self.IsoentalpicaGrosor)
        self.IsoentalpicaLinea = QtGui.QComboBox(self.page_3)
        self.IsoentalpicaLinea.setFixedWidth(60)
        self.IsoentalpicaLinea.setIconSize(QtCore.QSize(35, 18))
        self.horizontalLayout_Isoentalpica.addWidget(self.IsoentalpicaLinea)
        self.IsoentalpicaColorButton = QtGui.QToolButton(self.page_3)
        self.horizontalLayout_Isoentalpica.addWidget(self.IsoentalpicaColorButton)
        self.Isolineas.addTab(self.page_3, "")
        self.page_4 = QtGui.QWidget()
        self.gridLayout_5 = QtGui.QGridLayout(self.page_4)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.verticalLayout_10 = QtGui.QVBoxLayout()
        self.label1_4 = QtGui.QLabel(self.page_4)
        self.label1_4.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_10.addWidget(self.label1_4)
        self.S_ini = QtGui.QLineEdit(self.page_4)
        self.S_ini.setFixedSize(QtCore.QSize(80, 22))
        self.S_ini.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_10.addWidget(self.S_ini)
        self.horizontalLayout_4.addLayout(self.verticalLayout_10)
        self.verticalLayout_11 = QtGui.QVBoxLayout()
        self.label2_4 = QtGui.QLabel(self.page_4)
        self.label2_4.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_11.addWidget(self.label2_4)
        self.S_fin = QtGui.QLineEdit(self.page_4)
        self.S_fin.setFixedSize(QtCore.QSize(80, 22))
        self.S_fin.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_11.addWidget(self.S_fin)
        self.horizontalLayout_4.addLayout(self.verticalLayout_11)
        self.verticalLayout_12 = QtGui.QVBoxLayout()
        self.label3_4 = QtGui.QLabel(self.page_4)
        self.label3_4.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_12.addWidget(self.label3_4)
        self.S_intervalo = QtGui.QLineEdit(self.page_4)
        self.S_intervalo.setFixedSize(QtCore.QSize(80, 22))
        self.S_intervalo.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_12.addWidget(self.S_intervalo)
        self.horizontalLayout_4.addLayout(self.verticalLayout_12)
        self.gridLayout_5.addLayout(self.horizontalLayout_4, 0, 0, 1, 3)
        self.PersonalizarIsoentropica = QtGui.QCheckBox(self.page_4)
        self.gridLayout_5.addWidget(self.PersonalizarIsoentropica, 2, 0, 1, 1)
        self.ListaIsoentropica = QtGui.QLineEdit(self.page_4)
        self.gridLayout_5.addWidget(self.ListaIsoentropica, 2, 1, 1, 2)
        self.IsoentropicaCritica = QtGui.QCheckBox(self.page_4)
        self.gridLayout_5.addWidget(self.IsoentropicaCritica, 3, 0, 1, 2)
        self.horizontalLayout_Isoentropica= QtGui.QHBoxLayout()
        self.gridLayout_5.addLayout(self.horizontalLayout_Isoentropica, 4, 0, 1, 3)
        self.label_34 = QtGui.QLabel(self.page_4)
        self.label_34.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.horizontalLayout_Isoentropica.addWidget(self.label_34)
        self.IsoentropicaGrosor = QtGui.QDoubleSpinBox(self.page_4)
        self.IsoentropicaGrosor.setFixedWidth(50)
        self.IsoentropicaGrosor.setAlignment(QtCore.Qt.AlignRight)
        self.IsoentropicaGrosor.setRange(0.1, 5)
        self.IsoentropicaGrosor.setDecimals(1)
        self.IsoentropicaGrosor.setSingleStep(0.1)
        self.horizontalLayout_Isoentropica.addWidget(self.IsoentropicaGrosor)
        self.IsoentropicaLinea = QtGui.QComboBox(self.page_4)
        self.IsoentropicaLinea.setFixedWidth(60)
        self.IsoentropicaLinea.setIconSize(QtCore.QSize(35, 18))
        self.horizontalLayout_Isoentropica.addWidget(self.IsoentropicaLinea)
        self.IsoentropicaColorButton = QtGui.QToolButton(self.page_4)
        self.horizontalLayout_Isoentropica.addWidget(self.IsoentropicaColorButton)        
        self.Isolineas.addTab(self.page_4, "")
        self.page_5 = QtGui.QWidget()
        self.gridLayout_6 = QtGui.QGridLayout(self.page_5)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.verticalLayout_13 = QtGui.QVBoxLayout()
        self.label1_5 = QtGui.QLabel(self.page_5)
        self.label1_5.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_13.addWidget(self.label1_5)
        self.V_ini = QtGui.QLineEdit(self.page_5)
        self.V_ini.setFixedSize(QtCore.QSize(80, 22))
        self.V_ini.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_13.addWidget(self.V_ini)
        self.horizontalLayout_5.addLayout(self.verticalLayout_13)
        self.verticalLayout_14 = QtGui.QVBoxLayout()
        self.label2_5 = QtGui.QLabel(self.page_5)
        self.label2_5.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_14.addWidget(self.label2_5)
        self.V_fin = QtGui.QLineEdit(self.page_5)
        self.V_fin.setFixedSize(QtCore.QSize(80, 22))
        self.V_fin.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_14.addWidget(self.V_fin)
        self.horizontalLayout_5.addLayout(self.verticalLayout_14)
        self.verticalLayout_15 = QtGui.QVBoxLayout()
        self.label3_5 = QtGui.QLabel(self.page_5)
        self.label3_5.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_15.addWidget(self.label3_5)
        self.V_intervalo = QtGui.QLineEdit(self.page_5)
        self.V_intervalo.setFixedSize(QtCore.QSize(80, 22))
        self.V_intervalo.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_15.addWidget(self.V_intervalo)
        self.horizontalLayout_5.addLayout(self.verticalLayout_15)
        self.gridLayout_6.addLayout(self.horizontalLayout_5, 0, 0, 1, 3)
        self.PersonalizarIsocora = QtGui.QCheckBox(self.page_5)
        self.gridLayout_6.addWidget(self.PersonalizarIsocora, 2, 0, 1, 1)
        self.ListaIsocora = QtGui.QLineEdit(self.page_5)
        self.gridLayout_6.addWidget(self.ListaIsocora, 2, 1, 1, 2)
        self.IsocoraCritica = QtGui.QCheckBox(self.page_5)
        self.gridLayout_6.addWidget(self.IsocoraCritica, 3, 0, 1, 2)
        self.horizontalLayout_Isocora= QtGui.QHBoxLayout()
        self.gridLayout_6.addLayout(self.horizontalLayout_Isocora, 4, 0, 1, 3)
        self.label_35 = QtGui.QLabel(self.page_5)
        self.label_35.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.horizontalLayout_Isocora.addWidget(self.label_35)
        self.IsocoraGrosor = QtGui.QDoubleSpinBox(self.page_5)
        self.IsocoraGrosor.setFixedWidth(50)
        self.IsocoraGrosor.setAlignment(QtCore.Qt.AlignRight)
        self.IsocoraGrosor.setRange(0.1, 5)
        self.IsocoraGrosor.setDecimals(1)
        self.IsocoraGrosor.setSingleStep(0.1)
        self.horizontalLayout_Isocora.addWidget(self.IsocoraGrosor)
        self.IsocoraLinea = QtGui.QComboBox(self.page_5)
        self.IsocoraLinea.setFixedWidth(60)
        self.IsocoraLinea.setIconSize(QtCore.QSize(35, 18))
        self.horizontalLayout_Isocora.addWidget(self.IsocoraLinea)
        self.IsocoraColorButton = QtGui.QToolButton(self.page_5)
        self.horizontalLayout_Isocora.addWidget(self.IsocoraColorButton)           
        self.Isolineas.addTab(self.page_5, "")
        self.page_6 = QtGui.QWidget()
        self.gridLayout_15 = QtGui.QGridLayout(self.page_6)
        self.horizontalLayout_6 = QtGui.QHBoxLayout()
        self.verticalLayout_16 = QtGui.QVBoxLayout()
        self.label1_6 = QtGui.QLabel(self.page_6)
        self.label1_6.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_16.addWidget(self.label1_6)
        self.X_ini = QtGui.QLineEdit(self.page_6)
        self.X_ini.setFixedSize(QtCore.QSize(80, 22))
        self.X_ini.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_16.addWidget(self.X_ini)
        self.horizontalLayout_6.addLayout(self.verticalLayout_16)
        self.verticalLayout_17 = QtGui.QVBoxLayout()
        self.label2_6 = QtGui.QLabel(self.page_6)
        self.label2_6.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_17.addWidget(self.label2_6)
        self.X_fin = QtGui.QLineEdit(self.page_6)
        self.X_fin.setFixedSize(QtCore.QSize(80, 22))
        self.X_fin.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_17.addWidget(self.X_fin)
        self.horizontalLayout_6.addLayout(self.verticalLayout_17)
        self.verticalLayout_18 = QtGui.QVBoxLayout()
        self.label3_6 = QtGui.QLabel(self.page_6)
        self.label3_6.setAlignment(QtCore.Qt.AlignCenter|QtCore.Qt.AlignBottom)
        self.verticalLayout_18.addWidget(self.label3_6)
        self.X_intervalo = QtGui.QLineEdit(self.page_6)
        self.X_intervalo.setFixedSize(QtCore.QSize(80, 22))
        self.X_intervalo.setAlignment(QtCore.Qt.AlignRight)
        self.verticalLayout_18.addWidget(self.X_intervalo)
        self.horizontalLayout_6.addLayout(self.verticalLayout_18)
        self.gridLayout_15.addLayout(self.horizontalLayout_6, 0, 0, 1, 3)
        self.PersonalizarIsoX = QtGui.QCheckBox(self.page_6)
        self.gridLayout_15.addWidget(self.PersonalizarIsoX, 2, 0, 1, 1)
        self.ListaIsoX = QtGui.QLineEdit(self.page_6)
        self.gridLayout_15.addWidget(self.ListaIsoX, 2, 1, 1, 2)
        self.gridLayout_15.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),3, 0, 1, 3)
        self.horizontalLayout_IsoX= QtGui.QHBoxLayout()
        self.gridLayout_15.addLayout(self.horizontalLayout_IsoX, 4, 0, 1, 3)
        self.label_40 = QtGui.QLabel(self.page_6)
        self.label_40.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.horizontalLayout_IsoX.addWidget(self.label_40)
        self.IsoXGrosor = QtGui.QDoubleSpinBox(self.page_6)
        self.IsoXGrosor.setFixedWidth(50)
        self.IsoXGrosor.setAlignment(QtCore.Qt.AlignRight)
        self.IsoXGrosor.setRange(0.1, 5)
        self.IsoXGrosor.setDecimals(1)
        self.IsoXGrosor.setSingleStep(0.1)
        self.horizontalLayout_IsoX.addWidget(self.IsoXGrosor)
        self.IsoXLinea = QtGui.QComboBox(self.page_6)
        self.IsoXLinea.setFixedWidth(60)
        self.IsoXLinea.setIconSize(QtCore.QSize(35, 18))
        self.horizontalLayout_IsoX.addWidget(self.IsoXLinea)
        self.IsoXColorButton = QtGui.QToolButton(self.page_6)
        self.horizontalLayout_IsoX.addWidget(self.IsoXColorButton)           
        self.Isolineas.addTab(self.page_6, "")

        self.gridLayout_7.addWidget(self.Isolineas, 0, 0, 1, 2)
        self.botonActualizar = QtGui.QPushButton(self.dockWidgetContents_1)
        self.botonActualizar.setLayoutDirection(QtCore.Qt.RightToLeft)
        self.botonActualizar.setMaximumSize(QtCore.QSize(80,16777215))
        self.gridLayout_7.addWidget(self.botonActualizar,1,1,1,1)

        self.dockWidget_Isolineas.setWidget(self.dockWidgetContents_1)
        SteamTables.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget_Isolineas)
        self.dockWidget_Isolineas.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea)
        self.dockWidget_Isolineas.setFeatures(QtGui.QDockWidget.DockWidgetMovable)
        SteamTables.tabifyDockWidget(self.dockWidget_Tabla, self.dockWidget_Isolineas)
        SteamTables.setTabPosition(QtCore.Qt.DockWidgetArea(1), 0)
        
        lineas=[":/button/line.png", ":/button/dash.png", ":/button/line-dot.png", ":/button/dot.png"]
        for i in lineas:
            self.IsotermaLinea.addItem(QtGui.QIcon(QtGui.QPixmap(i)), "")
            self.IsobaraLinea.addItem(QtGui.QIcon(QtGui.QPixmap(i)), "")
            self.IsocoraLinea.addItem(QtGui.QIcon(QtGui.QPixmap(i)), "")
            self.IsoXLinea.addItem(QtGui.QIcon(QtGui.QPixmap(i)), "")
            self.IsoentalpicaLinea.addItem(QtGui.QIcon(QtGui.QPixmap(i)), "")
            self.IsoentropicaLinea.addItem(QtGui.QIcon(QtGui.QPixmap(i)), "")
        
        #Toolbox configuracion unidades
        self.dockWidget_Units = QtGui.QDockWidget(self.centralwidget)
        self.dockWidgetContents_Units = QtGui.QWidget()
        self.gridLayout_11 = QtGui.QGridLayout(self.dockWidgetContents_Units)

        self.groupBox = QtGui.QGroupBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.groupBox, 0, 1, 1, 5)
        self.gridLayout_12 = QtGui.QGridLayout(self.groupBox)
        self.SI = QtGui.QRadioButton(self.groupBox)
        self.gridLayout_12.addWidget(self.SI, 0, 1, 1, 1)
        self.English = QtGui.QRadioButton(self.groupBox)
        self.gridLayout_12.addWidget(self.English, 0, 2, 1, 1)
        self.gridLayout_11.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),1,0,1,6)
        self.label_unit_presion = QtGui.QLabel(self.dockWidgetContents_Units)
        self.label_unit_presion.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_unit_presion.setText("P")
        self.gridLayout_11.addWidget(self.label_unit_presion, 2, 1, 1, 1)
        self.conf_presion = QtGui.QComboBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.conf_presion, 2, 2, 1, 1)
        self.label_unit_temperatura = QtGui.QLabel(self.dockWidgetContents_Units)
        self.label_unit_temperatura.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_unit_temperatura.setText("T")
        self.gridLayout_11.addWidget(self.label_unit_temperatura, 3, 1, 1, 1)
        self.conf_temperatura = QtGui.QComboBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.conf_temperatura, 3, 2, 1, 1)
        self.label_unit_volumen = QtGui.QLabel(self.dockWidgetContents_Units)
        self.label_unit_volumen.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_unit_volumen.setText("V")
        self.gridLayout_11.addWidget(self.label_unit_volumen, 4, 1, 1, 1)
        self.conf_volumen = QtGui.QComboBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.conf_volumen, 4, 2, 1, 1)
        self.label_unit_entalpia = QtGui.QLabel(self.dockWidgetContents_Units)
        self.label_unit_entalpia.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_unit_entalpia.setText("H")
        self.gridLayout_11.addWidget(self.label_unit_entalpia, 5, 1, 1, 1)
        self.conf_entalpia = QtGui.QComboBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.conf_entalpia, 5, 2, 1, 1)
        self.label_unit_entropia = QtGui.QLabel(self.dockWidgetContents_Units)
        self.label_unit_entropia.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_unit_entropia.setText("S")
        self.gridLayout_11.addWidget(self.label_unit_entropia, 6, 1, 1, 1)
        self.conf_entropia= QtGui.QComboBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.conf_entropia, 6, 2, 1, 1)
        self.label_unit_densidad = QtGui.QLabel(self.dockWidgetContents_Units)
        self.label_unit_densidad.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_unit_densidad.setText(u"ρ")
        self.gridLayout_11.addWidget(self.label_unit_densidad, 2, 4, 1, 1)
        self.conf_densidad = QtGui.QComboBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.conf_densidad, 2, 5, 1, 1)
        self.label_unit_conductividad = QtGui.QLabel(self.dockWidgetContents_Units)
        self.label_unit_conductividad.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_unit_conductividad.setText("k")
        self.gridLayout_11.addWidget(self.label_unit_conductividad, 3, 4, 1, 1)
        self.conf_conductividad = QtGui.QComboBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.conf_conductividad, 3, 5, 1, 1)
        self.label_unit_capacidad = QtGui.QLabel(self.dockWidgetContents_Units)
        self.label_unit_capacidad.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_unit_capacidad.setText("Cp")
        self.gridLayout_11.addWidget(self.label_unit_capacidad, 4, 4, 1, 1)
        self.conf_capacidad = QtGui.QComboBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.conf_capacidad, 4, 5, 1, 1)
        self.label_unit_viscosidad = QtGui.QLabel(self.dockWidgetContents_Units)
        self.label_unit_viscosidad.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_unit_viscosidad.setText(u"µ")
        self.gridLayout_11.addWidget(self.label_unit_viscosidad, 5, 4, 1, 1)
        self.conf_viscosidad = QtGui.QComboBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.conf_viscosidad, 5, 5, 1, 1)
        self.label_unit_velocidad = QtGui.QLabel(self.dockWidgetContents_Units)
        self.label_unit_velocidad.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_unit_velocidad.setText("W")
        self.gridLayout_11.addWidget(self.label_unit_velocidad, 6, 4, 1, 1)
        self.conf_velocidad = QtGui.QComboBox(self.dockWidgetContents_Units)
        self.gridLayout_11.addWidget(self.conf_velocidad, 6, 5, 1, 1)

        for i in unidades.texto['Temperature']:
            self.conf_temperatura.addItem(i)
        for i in unidades.texto['Pressure']:
            self.conf_presion.addItem(i)
        for i in unidades.texto['SpecificVolume']:
            self.conf_volumen.addItem(i)
        for i in unidades.texto['Enthalpy']:
            self.conf_entalpia.addItem(i)
        for i in unidades.texto['SpecificHeat']:
            self.conf_entropia.addItem(i)
            self.conf_capacidad.addItem(i)
        for i in unidades.texto['Density']:
            self.conf_densidad.addItem(i)
        for i in unidades.texto['ThermalConductivity']:
            self.conf_conductividad.addItem(i)
        for i in unidades.texto['Viscosity']:
            self.conf_viscosidad.addItem(i)
        for i in unidades.texto['Speed']:
            self.conf_velocidad.addItem(i)

        self.dockWidget_Units.setWidget(self.dockWidgetContents_Units)
        SteamTables.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget_Units)
        self.dockWidget_Units.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea)
        self.dockWidget_Units.setFeatures(QtGui.QDockWidget.DockWidgetMovable)
        SteamTables.tabifyDockWidget(self.dockWidget_Tabla, self.dockWidget_Units)

        #Toolbox graficos 2D
        self.dockWidget_2D = QtGui.QDockWidget(self.centralwidget)
        self.tabEjes=QtGui.QTabWidget(self.dockWidget_2D)
        self.groupBox_3= QtGui.QWidget()
        self.gridLayout_16 = QtGui.QGridLayout(self.groupBox_3)
        self.rejilla=QtGui.QCheckBox(self.groupBox_3)
        self.gridLayout_16.addWidget(self.rejilla,0,0,1,2)
        self.tabEjes.addTab(self.groupBox_3, "")

        self.groupBox_1= QtGui.QWidget()
        self.gridLayout_13 = QtGui.QGridLayout(self.groupBox_1)
        self.label_28=QtGui.QLabel(self.groupBox_1)
        self.gridLayout_13.addWidget(self.label_28,0,0,1,1)
        self.ejeX = QtGui.QComboBox(self.groupBox_1)
        self.ejeX.setFixedWidth(50)
        self.gridLayout_13.addWidget(self.ejeX,0,1,1,1)
        self.label_36=QtGui.QLabel(self.groupBox_1)
        self.gridLayout_13.addWidget(self.label_36,1,0,1,1)
        self.ejeX_min=QtGui.QLineEdit(self.groupBox_1)
        self.ejeX_min.setValidator(QtGui.QDoubleValidator(self.groupBox_1))
        self.ejeX_min.setAlignment(QtCore.Qt.AlignRight)
        self.ejeX_min.setFixedWidth(50)
        self.gridLayout_13.addWidget(self.ejeX_min,1,1,1,1)
        self.label_37=QtGui.QLabel(self.groupBox_1)
        self.gridLayout_13.addWidget(self.label_37,1,2,1,1)
        self.ejeX_max=QtGui.QLineEdit(self.groupBox_1)
        self.ejeX_max.setValidator(QtGui.QDoubleValidator(self.groupBox_1))
        self.ejeX_max.setAlignment(QtCore.Qt.AlignRight)
        self.ejeX_max.setFixedWidth(50)
        self.gridLayout_13.addWidget(self.ejeX_max,1,3,1,1)
        self.ejeX_escala=QtGui.QCheckBox(self.groupBox_1)
        self.gridLayout_13.addWidget(self.ejeX_escala,2,0,1,4)
        self.label_29=QtGui.QLabel(self.groupBox_1)
        self.gridLayout_13.addWidget(self.label_29,3,0,1,1)
        self.ejeY = QtGui.QComboBox(self.groupBox_1)
        self.ejeY.setFixedWidth(50)
        self.gridLayout_13.addWidget(self.ejeY,3,1,1,1)
        self.label_38=QtGui.QLabel(self.groupBox_1)
        self.gridLayout_13.addWidget(self.label_38,4,0,1,1)
        self.ejeY_min=QtGui.QLineEdit(self.groupBox_1)
        self.ejeY_min.setValidator(QtGui.QDoubleValidator(self.groupBox_1))
        self.ejeY_min.setAlignment(QtCore.Qt.AlignRight)
        self.ejeY_min.setFixedWidth(50)
        self.gridLayout_13.addWidget(self.ejeY_min,4,1,1,1)
        self.label_39=QtGui.QLabel(self.groupBox_1)
        self.gridLayout_13.addWidget(self.label_39,4,2,1,1)
        self.ejeY_max=QtGui.QLineEdit(self.groupBox_1)
        self.ejeY_max.setValidator(QtGui.QDoubleValidator(self.groupBox_1))
        self.ejeY_max.setAlignment(QtCore.Qt.AlignRight)
        self.ejeY_max.setFixedWidth(50)
        self.gridLayout_13.addWidget(self.ejeY_max,4,3,1,1)
        self.ejeY_escala=QtGui.QCheckBox(self.groupBox_1)
        self.gridLayout_13.addWidget(self.ejeY_escala,5,0,1,4)
        self.tabEjes.addTab(self.groupBox_1, "")

        self.groupBox_2= QtGui.QWidget()
        self.gridLayout_14 = QtGui.QGridLayout(self.groupBox_2)
        self.checkIsoThermLabel=QtGui.QCheckBox(self.groupBox_2)
        self.gridLayout_14.addWidget(self.checkIsoThermLabel,0,0,1,1)
        self.label_41=QtGui.QLabel(self.groupBox_2)
        self.label_41.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.gridLayout_14.addWidget(self.label_41,0,1,1,1)
        self.PosIsoThermLabel=QtGui.QSpinBox(self.groupBox_2)
        self.PosIsoThermLabel.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.PosIsoThermLabel.setFixedWidth(45)
        self.gridLayout_14.addWidget(self.PosIsoThermLabel,0,2,1,1)
        self.checkIsoBarLabel=QtGui.QCheckBox(self.groupBox_2)
        self.gridLayout_14.addWidget(self.checkIsoBarLabel,1,0,1,1)
        self.label_42=QtGui.QLabel(self.groupBox_2)
        self.label_42.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.gridLayout_14.addWidget(self.label_42,1,1,1,1)
        self.PosIsoBarLabel=QtGui.QSpinBox(self.groupBox_2)
        self.PosIsoBarLabel.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.PosIsoBarLabel.setFixedWidth(45)
        self.gridLayout_14.addWidget(self.PosIsoBarLabel,1,2,1,1)
        self.checkIsoEnthLabel=QtGui.QCheckBox(self.groupBox_2)
        self.gridLayout_14.addWidget(self.checkIsoEnthLabel,2,0,1,1)
        self.label_43=QtGui.QLabel(self.groupBox_2)
        self.label_43.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.gridLayout_14.addWidget(self.label_43,2,1,1,1)
        self.PosIsoEnthLabel=QtGui.QSpinBox(self.groupBox_2)
        self.PosIsoEnthLabel.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.PosIsoEnthLabel.setFixedWidth(45)
        self.gridLayout_14.addWidget(self.PosIsoEnthLabel,2,2,1,1)
        self.checkIsoEntrLabel=QtGui.QCheckBox(self.groupBox_2)
        self.gridLayout_14.addWidget(self.checkIsoEntrLabel,3,0,1,1)
        self.label_44=QtGui.QLabel(self.groupBox_2)
        self.label_44.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.gridLayout_14.addWidget(self.label_44,3,1,1,1)
        self.PosIsoEntrLabel=QtGui.QSpinBox(self.groupBox_2)
        self.PosIsoEntrLabel.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.PosIsoEntrLabel.setFixedWidth(45)
        self.gridLayout_14.addWidget(self.PosIsoEntrLabel,3,2,1,1)
        self.checkIsoVolLabel=QtGui.QCheckBox(self.groupBox_2)
        self.gridLayout_14.addWidget(self.checkIsoVolLabel,4,0,1,1)
        self.label_45=QtGui.QLabel(self.groupBox_2)
        self.label_45.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.gridLayout_14.addWidget(self.label_45,4,1,1,1)
        self.PosIsoVolLabel=QtGui.QSpinBox(self.groupBox_2)
        self.PosIsoVolLabel.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.PosIsoVolLabel.setFixedWidth(45)
        self.gridLayout_14.addWidget(self.PosIsoVolLabel,4,2,1,1)
        self.checkIsoXLabel=QtGui.QCheckBox(self.groupBox_2)
        self.gridLayout_14.addWidget(self.checkIsoXLabel,5,0,1,1)
        self.label_46=QtGui.QLabel(self.groupBox_2)
        self.label_46.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.gridLayout_14.addWidget(self.label_46,5,1,1,1)
        self.PosIsoXLabel=QtGui.QSpinBox(self.groupBox_2)
        self.PosIsoXLabel.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.PosIsoXLabel.setFixedWidth(45)
        self.gridLayout_14.addWidget(self.PosIsoXLabel,5,2,1,1)
        self.tabEjes.addTab(self.groupBox_2, "")

        self.dockWidget_2D.setWidget(self.tabEjes)
        SteamTables.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget_2D)
        self.dockWidget_2D.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea)
        self.dockWidget_2D.setFeatures(QtGui.QDockWidget.DockWidgetMovable)
        SteamTables.tabifyDockWidget(self.dockWidget_Tabla, self.dockWidget_2D)

        
        #Toolbox parámetros Puntos individuales
        self.dockWidget_Puntos = QtGui.QDockWidget(self.centralwidget)
        self.dockWidgetContents_2 = QtGui.QWidget()
        self.gridLayout_1 = QtGui.QGridLayout(self.dockWidgetContents_2)
        self.label_1 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_1,0,0,1,1)
        self.variablesCalculo = QtGui.QComboBox(self.dockWidgetContents_2)
        self.variablesCalculo.setFixedWidth(100)
        self.gridLayout_1.addWidget(self.variablesCalculo,0,1,1,2)
        self.gridLayout_1.addItem(QtGui.QSpacerItem(50, 20, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Fixed), 1, 0, 1, 3)
        self.label_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_2,6,0,1,1)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setSpacing(0)
        self.horizontalLayout.setSizeConstraint(QtGui.QLayout.SetDefaultConstraint)
        self.presion = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.presion.setFixedSize(QtCore.QSize(85,24))
        self.presion.setAlignment(QtCore.Qt.AlignRight)
        self.presion.setValidator(QtGui.QDoubleValidator(self.dockWidgetContents_2))
        self.horizontalLayout.addWidget(self.presion)
        self.unidadesPresion = QtGui.QToolButton(self.dockWidgetContents_2)
        self.unidadesPresion.setFixedSize(QtCore.QSize(24,24))
        self.unidadesPresion.setText("...")
        self.horizontalLayout.addWidget(self.unidadesPresion)
        self.gridLayout_1.addLayout(self.horizontalLayout,6,1,1,1)
        self.label_2_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_2_2,6,2,1,1)
        self.label_3 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_3,7,0,1,1)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setSpacing(0)
        self.temperatura = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.temperatura.setFixedSize(QtCore.QSize(85,24))
        self.temperatura.setAlignment(QtCore.Qt.AlignRight)
        self.temperatura.setValidator(QtGui.QDoubleValidator(self.dockWidgetContents_2))
        self.horizontalLayout_2.addWidget(self.temperatura)        
        self.unidadesTemperatura = QtGui.QToolButton(self.dockWidgetContents_2)
        self.unidadesTemperatura.setFixedSize(QtCore.QSize(24,24))
        self.unidadesTemperatura.setText("...")
        self.horizontalLayout_2.addWidget(self.unidadesTemperatura)
        self.gridLayout_1.addLayout(self.horizontalLayout_2,7,1,1,1)
        self.label_3_2=QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_3_2,7,2,1,1)
        self.label_8 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_8,8,0,1,1)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setSpacing(0)
        self.volumen = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.volumen.setFixedSize(QtCore.QSize(85,24))
        self.volumen.setAlignment(QtCore.Qt.AlignRight)
        self.volumen.setValidator(QtGui.QDoubleValidator(self.dockWidgetContents_2))
        self.horizontalLayout_3.addWidget(self.volumen)
        self.unidadesVolumen = QtGui.QToolButton(self.dockWidgetContents_2)
        self.unidadesVolumen.setFixedSize(QtCore.QSize(24,24))
        self.unidadesVolumen.setText("...")
        self.horizontalLayout_3.addWidget(self.unidadesVolumen)
        self.gridLayout_1.addLayout(self.horizontalLayout_3,8,1,1,1)
        self.label_8_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_8_2,8,2,1,1)
        self.label_4 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_4,9,0,1,1)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setSpacing(0)
        self.entalpia = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.entalpia.setFixedSize(QtCore.QSize(85,24))
        self.entalpia.setAlignment(QtCore.Qt.AlignRight)
        self.entalpia.setValidator(QtGui.QDoubleValidator(self.dockWidgetContents_2))
        self.horizontalLayout_4.addWidget(self.entalpia)
        self.unidadesEntalpia = QtGui.QToolButton(self.dockWidgetContents_2)
        self.unidadesEntalpia.setFixedSize(QtCore.QSize(24,24))
        self.unidadesEntalpia.setText("...")
        self.horizontalLayout_4.addWidget(self.unidadesEntalpia)
        self.gridLayout_1.addLayout(self.horizontalLayout_4,9,1,1,1)
        self.label_4_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_4_2,9,2,1,1)
        self.label_5 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_5,10,0,1,1)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setSpacing(0)
        self.entropia = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.entropia.setFixedSize(QtCore.QSize(85,24))
        self.entropia.setAlignment(QtCore.Qt.AlignRight)
        self.entropia.setValidator(QtGui.QDoubleValidator(self.dockWidgetContents_2))
        self.horizontalLayout_5.addWidget(self.entropia)
        self.unidadesEntropia = QtGui.QToolButton(self.dockWidgetContents_2)
        self.unidadesEntropia.setFixedSize(QtCore.QSize(24,24))
        self.unidadesEntropia.setText("...")
        self.horizontalLayout_5.addWidget(self.unidadesEntropia)
        self.gridLayout_1.addLayout(self.horizontalLayout_5,10,1,1,1)
        self.label_5_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_5_2,10,2,1,1)
        self.label_12 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_12,11,0,1,1)
        self.fraccionVapor = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.fraccionVapor.setFixedSize(QtCore.QSize(85,24))
        self.fraccionVapor.setAlignment(QtCore.Qt.AlignRight)
        self.fraccionVapor.setValidator(QtGui.QDoubleValidator(self.dockWidgetContents_2))
        self.gridLayout_1.addWidget(self.fraccionVapor,11,1,1,1)
        self.label_13 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_13,12,0,1,1)
        self.energiaInterna = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.energiaInterna.setFixedSize(QtCore.QSize(85,24))
        self.energiaInterna.setReadOnly(True)
        self.energiaInterna.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout_1.addWidget(self.energiaInterna,12,1,1,1)
        self.label_13_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_13_2,12,2,1,1)
        self.label_18 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_18,13,0,1,1)
        self.energiaGibbs = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.energiaGibbs.setFixedSize(QtCore.QSize(85,24))
        self.energiaGibbs.setReadOnly(True)
        self.energiaGibbs.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout_1.addWidget(self.energiaGibbs,13,1,1,1)
        self.label_18_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_18_2,13,2,1,1)
        self.label_19 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_19,14,0,1,1)
        self.energiaHelmholtz = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.energiaHelmholtz.setFixedSize(QtCore.QSize(85,24))
        self.energiaHelmholtz.setReadOnly(True)
        self.energiaHelmholtz.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout_1.addWidget(self.energiaHelmholtz,14,1,1,1)
        self.label_19_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_19_2,14,2,1,1)
        self.label_20 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_20,15,0,1,1)
        self.densidad = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.densidad.setFixedSize(QtCore.QSize(85,24))
        self.densidad.setReadOnly(True)
        self.densidad.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout_1.addWidget(self.densidad,15,1,1,1)
        self.label_20_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_20_2,15,2,1,1)
        self.label_21 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_21,16,0,1,1)
        self.cp = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.cp.setFixedSize(QtCore.QSize(85,24))
        self.cp.setReadOnly(True)
        self.cp.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout_1.addWidget(self.cp,16,1,1,1)
        self.label_21_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_21_2,16,2,1,1)
        self.label_22 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_22,17,0,1,1)
        self.cv = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.cv.setFixedSize(QtCore.QSize(85,24))
        self.cv.setReadOnly(True)
        self.cv.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout_1.addWidget(self.cv,17,1,1,1)
        self.label_22_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_22_2,17,2,1,1)
        self.label_23 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_23,18,0,1,1)
        self.conductividad = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.conductividad.setFixedSize(QtCore.QSize(85,24))
        self.conductividad.setReadOnly(True)
        self.conductividad.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout_1.addWidget(self.conductividad,18,1,1,1)
        self.label_23_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_23_2,18,2,1,1)
        self.label_24 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_24,19,0,1,1)
        self.viscosidad = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.viscosidad.setFixedSize(QtCore.QSize(85,24))
        self.viscosidad.setReadOnly(True)
        self.viscosidad.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout_1.addWidget(self.viscosidad, 19,1,1,1)
        self.label_24_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_24_2,19,2,1,1)
        self.label_25 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_25,20,0,1,1)
        self.velocidad = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.velocidad.setFixedSize(QtCore.QSize(85,24))
        self.velocidad.setReadOnly(True)
        self.velocidad.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout_1.addWidget(self.velocidad,20,1,1,1)
        self.label_25_2 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_25_2,20,2,1,1)
        self.label_30 = QtGui.QLabel(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.label_30,21,0,1,1)
        self.region = QtGui.QLineEdit(self.dockWidgetContents_2)
        self.region.setFixedSize(QtCore.QSize(85,24))
        self.region.setReadOnly(True)
        self.region.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout_1.addWidget(self.region,21,1,1,1)
        self.botonAdd=QtGui.QPushButton(self.dockWidgetContents_2)
        self.gridLayout_1.addWidget(self.botonAdd,22,0,1,1)
        self.botonAdd.setEnabled(False)
        self.botonLista=QtGui.QPushButton(self.dockWidgetContents_2)
        self.botonLista.setMaximumSize(QtCore.QSize(80,16777215))
        self.gridLayout_1.addWidget(self.botonLista,22,1,1,2)
        spacerItem = QtGui.QSpacerItem(72, 34, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.gridLayout_1.addItem(spacerItem, 23, 1, 1, 3)
        self.mostrarUnidades()
        self.dockWidget_Puntos.setWidget(self.dockWidgetContents_2)
        self.dockWidget_Puntos.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea)
        SteamTables.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget_Puntos)
        self.dockWidget_Puntos.setFeatures(QtGui.QDockWidget.DockWidgetMovable)
        self.dockWidget_Puntos.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea)
        
        #Iniciar valores de widgets
        Ejes=["p,T", "p,h", "p,s", "p,v", "T,s", "T,x"]
        for i in Ejes:
            self.ejesTabla.addItem(i)
            self.variablesCalculo.addItem(i)
        Ejes2D=["p", "T", "s", "h", "u", "v"]
        for i in Ejes2D:
            self.ejeX.addItem(i)
            self.ejeY.addItem(i)
            
        self.dockWidget_Tabla.raise_()
        self.matriz=[]
        self.saturacion=[]
        self.isoterma=[]
        self.isobara=[]
        self.isoentropica=[]
        self.isoentalpica=[]
        self.isocora=[]
        self.isoX=[]
        self.factorx, self.factory, self.factorz, self.factorx2, self.factory2=(0, 0, 0, 0, 0)
        
        #Cargar configuración
        Config=ConfigParser()
        Config.read("UI_steamTablesrc")
        if Config.has_section("Table"):
            self.ejesTabla.setCurrentIndex(Config.getint("Table","Axis"))
            self.ejesTabla_currentIndexChanged(Config.getint("Table","Axis"))
            self.variableTabla.setCurrentIndex(Config.getint("Table","Calculate"))
            self.abscisaInicio.setText(Config.get("Table","x_start"))
            self.abscisaFin.setText(Config.get("Table","x_end"))
            self.abscisaIntervalo.setText(Config.get("Table","x_step"))
            self.ordenadaInicio.setText(Config.get("Table","y_start"))
            self.ordenadaFin.setText(Config.get("Table","y_end"))
            self.ordenadaIntervalo.setText(Config.get("Table","y_step"))
        if Config.has_section("Units"):
            self.conf_temperatura.setCurrentIndex(Config.getint("Units","Temperature"))
            self.conf_presion.setCurrentIndex(Config.getint("Units","Pressure"))
            self.conf_volumen.setCurrentIndex(Config.getint("Units","SpecificVolume"))
            self.conf_entalpia.setCurrentIndex(Config.getint("Units","Enthalpy"))
            self.conf_entropia.setCurrentIndex(Config.getint("Units","Entropy"))
            self.conf_densidad.setCurrentIndex(Config.getint("Units","Density"))
            self.conf_conductividad.setCurrentIndex(Config.getint("Units","ThermalConductivity"))
            self.conf_capacidad.setCurrentIndex(Config.getint("Units","SpecificHeat"))
            self.conf_viscosidad.setCurrentIndex(Config.getint("Units","Viscosity"))
            self.conf_velocidad.setCurrentIndex(Config.getint("Units","Speed"))
        if Config.has_section("General"):
            self.actionDibujarSaturacion.setChecked(Config.getboolean("General","Sat"))
            self.actionMostrarPuntos.setChecked(Config.getboolean("General","Points"))
            self.actionMostrarBarra.setChecked(Config.getboolean("General","Toolbar"))
            self.toolbar2D.setVisible(self.actionMostrarBarra.isChecked())
            self.toolbar3D.setVisible(self.actionMostrarBarra.isChecked())
            self.action2D.setChecked(Config.getboolean("General","2D"))
            self.d2(self.action2D.isChecked())
            self.d3(not self.action2D.isChecked())
            if Config.getboolean("General","Plot"):
                self.toolBox.setCurrentIndex(1)
            self.checkIsoTherm.setChecked(Config.getboolean("General","Isotherm"))
            self.checkIsoBar.setChecked(Config.getboolean("General","Isobar"))
            self.checkIsoEnth.setChecked(Config.getboolean("General","Isoenthalpic"))
            self.checkIsoEntr.setChecked(Config.getboolean("General","Isoentropic"))
            self.checkIsoVol.setChecked(Config.getboolean("General","Isochor"))
            self.checkIsoX.setChecked(Config.getboolean("General","Isoquality"))
        if Config.has_section("2D"):
            self.ejeX.setCurrentIndex(self.ejeX.findText(Config.get("2D", "Xvariable")))
            self.rellenar_ejeY(self.ejeX.currentIndex())
            self.ejeX_escala.setChecked(Config.getboolean("2D", "XScale"))
            self.ejeX_min.setText(Config.get("2D", "XMin"))
            self.ejeX_max.setText(Config.get("2D", "XMax"))
            self.ejeY.setCurrentIndex(self.ejeY.findText(Config.get("2D", "Yvariable")))
            self.ejeY_escala.setChecked(Config.getboolean("2D", "YScale"))
            self.ejeY_min.setText(Config.get("2D", "YMin"))
            self.ejeY_max.setText(Config.get("2D", "YMax"))
            self.rejilla.setChecked(Config.getboolean("2D", "Grid"))
            self.checkIsoThermLabel.setChecked(Config.getboolean("2D", "IsothermLabel"))
            self.PosIsoThermLabel.setValue(Config.getfloat("2D", "IsothermLabelPos"))
            self.checkIsoBarLabel.setChecked(Config.getboolean("2D", "IsobarLabel"))
            self.PosIsoBarLabel.setValue(Config.getfloat("2D", "IsobarLabelPos"))
            self.checkIsoEnthLabel.setChecked(Config.getboolean("2D", "IsoenthLabel"))
            self.PosIsoEnthLabel.setValue(Config.getfloat("2D", "IsoenthLabelPos"))
            self.checkIsoEntrLabel.setChecked(Config.getboolean("2D", "IsoentrLabel"))
            self.PosIsoEntrLabel.setValue(Config.getfloat("2D", "IsoentrLabelPos"))
            self.checkIsoVolLabel.setChecked(Config.getboolean("2D", "IsochorLabel"))
            self.PosIsoVolLabel.setValue(Config.getfloat("2D", "IsochorLabelPos"))
            self.checkIsoXLabel.setChecked(Config.getboolean("2D", "IsoXLabel"))
            self.PosIsoXLabel.setValue(Config.getfloat("2D", "IsoXLabelPos"))
        if Config.has_section("Isotherm"):
            self.T_ini.setText(str(unidades.Temperature(Config.getfloat("Isotherm", 'Start')).config))
            self.T_fin.setText(str(unidades.Temperature(Config.getfloat("Isotherm", 'End')).config))
            self.T_intervalo.setText(str(unidades.DeltaT(Config.getfloat("Isotherm", 'Step')).config))
            self.PersonalizarIsoterma.setChecked(Config.getboolean("Isotherm", 'Custom'))
            if Config.get("Isotherm", 'List')<>"":
                T=[]
                for i in Config.get("Isotherm", 'List').split(','):
                    T.append(config.representacion(unidades.Temperature(float(i)).config))
                self.ListaIsoterma.setText(",".join(T))
            self.IsotermaCritica.setChecked(Config.getboolean("Isotherm", 'Critic'))
            self.IsotermaColor=QtGui.QColor(Config.get("Isotherm", 'Color'))
            self.IsotermaColorButton.setPalette(QtGui.QPalette(self.IsotermaColor))
            self.T_ini.setDisabled(self.PersonalizarIsoterma.isChecked())
            self.T_fin.setDisabled(self.PersonalizarIsoterma.isChecked())
            self.T_intervalo.setDisabled(self.PersonalizarIsoterma.isChecked())
            self.ListaIsoterma.setEnabled(self.PersonalizarIsoterma.isChecked())
            self.IsotermaGrosor.setValue(Config.getfloat("Isotherm", 'lineWidth'))
            self.IsotermaLinea.setCurrentIndex(Config.getint("Isotherm", 'lineStyle'))
        if Config.has_section("Isobar"):
            self.P_ini.setText(str(unidades.Pressure(Config.getfloat("Isobar", 'Start')).config))
            self.P_fin.setText(str(unidades.Pressure(Config.getfloat("Isobar", 'End')).config))
            self.P_intervalo.setText(str(unidades.Pressure(Config.getfloat("Isobar", 'Step')).config))
            self.PersonalizarIsobara.setChecked(Config.getboolean("Isobar", 'Custom'))
            if Config.get("Isobar", 'List')<>"":
                P=[]
                for i in Config.get("Isobar", 'List').split(','):
                    P.append(config.representacion(unidades.Pressure(float(i)).config))
                self.ListaIsobara.setText(",".join(P))
            self.IsobaraCritica.setChecked(Config.getboolean("Isobar", 'Critic'))
            self.IsobaraColor=QtGui.QColor(Config.get("Isobar", 'Color'))
            self.IsobaraColorButton.setPalette(QtGui.QPalette(self.IsobaraColor))
            self.P_ini.setDisabled(self.PersonalizarIsobara.isChecked())
            self.P_fin.setDisabled(self.PersonalizarIsobara.isChecked())
            self.P_intervalo.setDisabled(self.PersonalizarIsobara.isChecked())
            self.ListaIsobara.setEnabled(self.PersonalizarIsobara.isChecked())
            self.IsobaraGrosor.setValue(Config.getfloat("Isobar", 'lineWidth'))
            self.IsobaraLinea.setCurrentIndex(Config.getint("Isobar", 'lineStyle'))
        if Config.has_section("Isochor"):
            self.V_ini.setText(str(unidades.SpecificVolume(Config.getfloat("Isochor", 'Start')).config))
            self.V_fin.setText(str(unidades.SpecificVolume(Config.getfloat("Isochor", 'End')).config))
            self.V_intervalo.setText(str(unidades.SpecificVolume(Config.getfloat("Isochor", 'Step')).config))
            self.PersonalizarIsocora.setChecked(Config.getboolean("Isochor", 'Custom'))
            if Config.get("Isochor", 'List')<>"":
                V=[]
                for i in Config.get("Isochor", 'List').split(','):
                    V.append(config.representacion(unidades.SpecificVolume(float(i)).config))
                self.ListaIsocora.setText(",".join(V))
            self.IsocoraCritica.setChecked(Config.getboolean("Isochor", 'Critic'))
            self.IsocoraColor=QtGui.QColor(Config.get("Isochor", 'Color'))
            self.IsocoraColorButton.setPalette(QtGui.QPalette(self.IsocoraColor))
            self.V_ini.setDisabled(self.PersonalizarIsocora.isChecked())
            self.V_fin.setDisabled(self.PersonalizarIsocora.isChecked())
            self.V_intervalo.setDisabled(self.PersonalizarIsocora.isChecked())
            self.ListaIsocora.setEnabled(self.PersonalizarIsocora.isChecked())
            self.IsocoraGrosor.setValue(Config.getfloat("Isochor", 'lineWidth'))
            self.IsocoraLinea.setCurrentIndex(Config.getint("Isochor", 'lineStyle'))
        if Config.has_section("Isoquality"):
            self.X_ini.setText(Config.get("Isoquality", 'Start'))
            self.X_fin.setText(Config.get("Isoquality", 'End'))
            self.X_intervalo.setText(Config.get("Isoquality", 'Step'))
            self.PersonalizarIsoX.setChecked(Config.getboolean("Isoquality", 'Custom'))
            self.ListaIsoX.setText(Config.get("Isoquality", 'List'))
            self.IsoXColor=QtGui.QColor(Config.get("Isoquality", 'Color'))
            self.IsoXColorButton.setPalette(QtGui.QPalette(self.IsoXColor))
            self.X_ini.setDisabled(self.PersonalizarIsoX.isChecked())
            self.X_fin.setDisabled(self.PersonalizarIsoX.isChecked())
            self.X_intervalo.setDisabled(self.PersonalizarIsoX.isChecked())
            self.ListaIsoX.setEnabled(self.PersonalizarIsoX.isChecked())
            self.IsoXGrosor.setValue(Config.getfloat("Isoquality", 'lineWidth'))
            self.IsoXLinea.setCurrentIndex(Config.getint("Isoquality", 'lineStyle'))
        if Config.has_section("Isoenthalpic"):
            self.H_ini.setText(str(unidades.Enthalpy(Config.getfloat("Isoenthalpic", 'Start')).config))
            self.H_fin.setText(str(unidades.Enthalpy(Config.getfloat("Isoenthalpic", 'End')).config))
            self.H_intervalo.setText(str(unidades.Enthalpy(Config.getfloat("Isoenthalpic", 'Step')).config))
            self.PersonalizarIsoentalpica.setChecked(Config.getboolean("Isoenthalpic", 'Custom'))          
            if Config.get("Isoenthalpic", 'List')<>"":
                H=[]
                for i in Config.get("Isoenthalpic", 'List').split(','):
                    H.append(config.representacion(unidades.Enthalpy(float(i)).config))
                self.ListaIsoentalpica.setText(",".join(H))
            self.IsoentalpicaCritica.setChecked(Config.getboolean("Isoenthalpic", 'Critic'))
            self.IsoentalpicaColor=QtGui.QColor(Config.get("Isoenthalpic", 'Color'))
            self.IsoentalpicaColorButton.setPalette(QtGui.QPalette(self.IsoentalpicaColor))
            self.H_ini.setDisabled(self.PersonalizarIsoentalpica.isChecked())
            self.H_fin.setDisabled(self.PersonalizarIsoentalpica.isChecked())
            self.H_intervalo.setDisabled(self.PersonalizarIsoentalpica.isChecked())
            self.ListaIsoentalpica.setEnabled(self.PersonalizarIsoentalpica.isChecked())
            self.IsoentalpicaGrosor.setValue(Config.getfloat("Isoenthalpic", 'lineWidth'))
            self.IsoentalpicaLinea.setCurrentIndex(Config.getint("Isoenthalpic", 'lineStyle'))
        if Config.has_section("Isoentropic"):
            self.S_ini.setText(str(unidades.SpecificHeat(Config.getfloat("Isoentropic", 'Start')).config("Entropy")))
            self.S_fin.setText(str(unidades.SpecificHeat(Config.getfloat("Isoentropic", 'End')).config("Entropy")))
            self.S_intervalo.setText(str(unidades.SpecificHeat(Config.getfloat("Isoentropic", 'Step')).config("Entropy")))
            self.PersonalizarIsoentropica.setChecked(Config.getboolean("Isoentropic", 'Custom'))
            if Config.get("Isoentropic", 'List')<>"":
                S=[]
                for i in Config.get("Isoentropic", 'List').split(','):
                    S.append(config.representacion(unidades.SpecificHeat(float(i)).config("Entropy")))
                self.ListaIsoentropica.setText(",".join(S))
            self.IsoentropicaCritica.setChecked(Config.getboolean("Isoentropic", 'Critic'))
            self.IsoentropicaColor=QtGui.QColor(Config.get("Isoentropic", 'Color'))
            self.IsoentropicaColorButton.setPalette(QtGui.QPalette(self.IsoentropicaColor))
            self.S_ini.setDisabled(self.PersonalizarIsoentropica.isChecked())
            self.S_fin.setDisabled(self.PersonalizarIsoentropica.isChecked())
            self.S_intervalo.setDisabled(self.PersonalizarIsoentropica.isChecked())
            self.ListaIsoentropica.setEnabled(self.PersonalizarIsoentropica.isChecked())
            self.IsoentropicaGrosor.setValue(Config.getfloat("Isoentropic", 'lineWidth'))
            self.IsoentropicaLinea.setCurrentIndex(Config.getint("Isoentropic", 'lineStyle'))
        if Config.has_section("Points"):
            self.variablesCalculo.setCurrentIndex(Config.getint("Points", 'Variable'))
            self.variablesCalculo_currentIndexChanged(Config.getint("Points", 'Variable'))
            i=0
            self.puntos=[]
            while True:
                try:
                    punto=map(float, Config.get("Points", str(i)).split(","))
                except: break
                if punto[0]==0.0:
                    self.puntos.append(steam_Tx(punto[1], punto[2]))
                else:
                    self.puntos.append(steam_pT(punto[2], punto[1]))
                i+=1
            self.punto=self.puntos[-1]
            self.botonAdd.setEnabled(True)
            self.mostrarPropiedades()

        self.retranslateUi(SteamTables)
        QtCore.QObject.connect(self.ejesTabla,QtCore.SIGNAL("currentIndexChanged(int)"),self.ejesTabla_currentIndexChanged)
        QtCore.QObject.connect(self.variablesCalculo,QtCore.SIGNAL("currentIndexChanged(int)"),self.variablesCalculo_currentIndexChanged)
        QtCore.QObject.connect(self.botonCalcular,QtCore.SIGNAL("clicked()"),self.botonCalcular_clicked)
        QtCore.QObject.connect(self.unidadesPresion,QtCore.SIGNAL("clicked()"),self.unidadesPresion_clicked)
        QtCore.QObject.connect(self.unidadesTemperatura,QtCore.SIGNAL("clicked()"),self.unidadesTemperatura_clicked)
        QtCore.QObject.connect(self.unidadesVolumen,QtCore.SIGNAL("clicked()"),self.unidadesVolumen_clicked)
        QtCore.QObject.connect(self.unidadesEntalpia,QtCore.SIGNAL("clicked()"),self.unidadesEntalpia_clicked)
        QtCore.QObject.connect(self.unidadesEntropia,QtCore.SIGNAL("clicked()"),self.unidadesEntropia_clicked)
        QtCore.QObject.connect(self.presion,QtCore.SIGNAL("editingFinished()"),self.presion_editingFinished)
        QtCore.QObject.connect(self.temperatura,QtCore.SIGNAL("editingFinished()"),self.temperatura_editingFinished)
        QtCore.QObject.connect(self.entalpia,QtCore.SIGNAL("editingFinished()"),self.entalpia_editingFinished)
        QtCore.QObject.connect(self.entropia,QtCore.SIGNAL("editingFinished()"),self.entropia_editingFinished)
        QtCore.QObject.connect(self.volumen,QtCore.SIGNAL("editingFinished()"),self.volumen_editingFinished)
        QtCore.QObject.connect(self.fraccionVapor,QtCore.SIGNAL("editingFinished()"),self.fraccionVapor_editingFinished)
        QtCore.QObject.connect(self.actionCSV,QtCore.SIGNAL("activated(int)"),self.exporttoCSV)
        QtCore.QObject.connect(self.actionSalir,QtCore.SIGNAL("activated(int)"),SteamTables.close)
        QtCore.QObject.connect(self.actionAcerca_de,QtCore.SIGNAL("activated(int)"),self.acerca)
        QtCore.QObject.connect(self.actionAcerca_deQt,QtCore.SIGNAL("activated(int)"),self.acercaQt)
#        QtCore.QObject.connect(self.variableTabla,QtCore.SIGNAL("currentIndexChanged(int)"),self.Calcular_Propiedades)
        QtCore.QObject.connect(self.ejeX,QtCore.SIGNAL("currentIndexChanged(int)"),self.rellenar_ejeY)
        QtCore.QObject.connect(self.ejeX_min,QtCore.SIGNAL("editingFinished()"),self.diagrama2D_ejeX)
        QtCore.QObject.connect(self.ejeX_max,QtCore.SIGNAL("editingFinished()"),self.diagrama2D_ejeX)
        QtCore.QObject.connect(self.ejeY_min,QtCore.SIGNAL("editingFinished()"),self.diagrama2D_ejeY)
        QtCore.QObject.connect(self.ejeY_max,QtCore.SIGNAL("editingFinished()"),self.diagrama2D_ejeY)
        QtCore.QObject.connect(self.ejeX_escala,QtCore.SIGNAL("toggled(bool)"),self.ejeX_log)
        QtCore.QObject.connect(self.ejeY_escala,QtCore.SIGNAL("toggled(bool)"),self.ejeY_log)
        QtCore.QObject.connect(self.rejilla,QtCore.SIGNAL("toggled(bool)"),self.rejilla_toggled)
        QtCore.QObject.connect(self.botonAdd,QtCore.SIGNAL("clicked()"),self.botonAdd_clicked)
        QtCore.QObject.connect(self.botonActualizar,QtCore.SIGNAL("clicked()"),self.botonActualizar_clicked)
        QtCore.QObject.connect(self.botonLista,QtCore.SIGNAL("clicked()"),self.botonLista_clicked)
        QtCore.QObject.connect(self.actionMostrarBarra,QtCore.SIGNAL("triggered(bool)"),self.mostrarBarra)
        QtCore.QObject.connect(self.actionMostrarPuntos,QtCore.SIGNAL("triggered(bool)"),self.mostrarPuntos)
        QtCore.QObject.connect(self.actionDibujarSaturacion,QtCore.SIGNAL("triggered(bool)"),self.mostrarSaturacion)
        QtCore.QObject.connect(self.checkIsoEntr,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsoentropica)
        QtCore.QObject.connect(self.checkIsoEnth,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsoentalpica)
        QtCore.QObject.connect(self.checkIsoTherm,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsoterma)
        QtCore.QObject.connect(self.checkIsoBar,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsobara)
        QtCore.QObject.connect(self.checkIsoVol,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsocora)
        QtCore.QObject.connect(self.checkIsoX,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsoX)
        QtCore.QObject.connect(self.PersonalizarIsoterma,QtCore.SIGNAL("toggled(bool)"),self.PersonalizarIsoterma_toggled)
        QtCore.QObject.connect(self.PersonalizarIsocora,QtCore.SIGNAL("toggled(bool)"),self.PersonalizarIsocora_toggled)
        QtCore.QObject.connect(self.PersonalizarIsoX,QtCore.SIGNAL("toggled(bool)"),self.PersonalizarIsoX_toggled)
        QtCore.QObject.connect(self.PersonalizarIsoentalpica,QtCore.SIGNAL("toggled(bool)"),self.PersonalizarIsoentalpica_toggled)
        QtCore.QObject.connect(self.PersonalizarIsoentropica,QtCore.SIGNAL("toggled(bool)"),self.PersonalizarIsoentropica_toggled)
        QtCore.QObject.connect(self.PersonalizarIsobara,QtCore.SIGNAL("toggled(bool)"),self.PersonalizarIsobara_toggled)
        QtCore.QObject.connect(self.IsotermaCritica,QtCore.SIGNAL("toggled(bool)"),self.IsotermaCritica_togggled)
        QtCore.QObject.connect(self.IsobaraCritica,QtCore.SIGNAL("toggled(bool)"),self.IsobaraCritica_togggled)
        QtCore.QObject.connect(self.IsoentalpicaCritica,QtCore.SIGNAL("toggled(bool)"),self.IsoentalpicaCritica_togggled)
        QtCore.QObject.connect(self.IsoentropicaCritica,QtCore.SIGNAL("toggled(bool)"),self.IsoentropicaCritica_togggled)
        QtCore.QObject.connect(self.IsocoraCritica,QtCore.SIGNAL("toggled(bool)"),self.IsocoraCritica_togggled)
        
        QtCore.QObject.connect(self.IsotermaGrosor,QtCore.SIGNAL("valueChanged(double)"),self.dibujar)
        QtCore.QObject.connect(self.IsotermaLinea,QtCore.SIGNAL("currentIndexChanged(int)"),self.dibujar)
        QtCore.QObject.connect(self.IsotermaColorButton,QtCore.SIGNAL("clicked()"),self.IsotermaColor_clicked)
        QtCore.QObject.connect(self.IsobaraGrosor,QtCore.SIGNAL("valueChanged(double)"),self.dibujar)
        QtCore.QObject.connect(self.IsobaraLinea,QtCore.SIGNAL("currentIndexChanged(int)"),self.dibujar)
        QtCore.QObject.connect(self.IsobaraColorButton,QtCore.SIGNAL("clicked()"),self.IsobaraColor_clicked)
        QtCore.QObject.connect(self.IsocoraGrosor,QtCore.SIGNAL("valueChanged(double)"),self.dibujar)
        QtCore.QObject.connect(self.IsocoraLinea,QtCore.SIGNAL("currentIndexChanged(int)"),self.dibujar)
        QtCore.QObject.connect(self.IsocoraColorButton,QtCore.SIGNAL("clicked()"),self.IsocoraColor_clicked)
        QtCore.QObject.connect(self.IsoXGrosor,QtCore.SIGNAL("valueChanged(double)"),self.dibujar)
        QtCore.QObject.connect(self.IsoXLinea,QtCore.SIGNAL("currentIndexChanged(int)"),self.dibujar)
        QtCore.QObject.connect(self.IsoXColorButton,QtCore.SIGNAL("clicked()"),self.IsoXColor_clicked)
        QtCore.QObject.connect(self.IsoentalpicaGrosor,QtCore.SIGNAL("valueChanged(double)"),self.dibujar)
        QtCore.QObject.connect(self.IsoentalpicaLinea,QtCore.SIGNAL("currentIndexChanged(int)"),self.dibujar)
        QtCore.QObject.connect(self.IsoentalpicaColorButton,QtCore.SIGNAL("clicked()"),self.IsoentalpicaColor_clicked)
        QtCore.QObject.connect(self.IsoentropicaGrosor,QtCore.SIGNAL("valueChanged(double)"),self.dibujar)
        QtCore.QObject.connect(self.IsoentropicaLinea,QtCore.SIGNAL("currentIndexChanged(int)"),self.dibujar)
        QtCore.QObject.connect(self.IsoentropicaColorButton,QtCore.SIGNAL("clicked()"),self.IsoentropicaColor_clicked)
        
        QtCore.QObject.connect(self.checkIsoThermLabel,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsoterma)
        QtCore.QObject.connect(self.checkIsoBarLabel,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsobara)
        QtCore.QObject.connect(self.checkIsoEnthLabel,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsoentalpica)
        QtCore.QObject.connect(self.checkIsoEntrLabel,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsoentropica)
        QtCore.QObject.connect(self.checkIsoVolLabel,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsocora)
        QtCore.QObject.connect(self.checkIsoXLabel,QtCore.SIGNAL("toggled(bool)"),self.mostrarIsoX)
        QtCore.QObject.connect(self.PosIsoThermLabel,QtCore.SIGNAL("editingFinished()"),self.IsotermaCritica_togggled)
        QtCore.QObject.connect(self.PosIsoBarLabel,QtCore.SIGNAL("editingFinished()"),self.IsobaraCritica_togggled)
        QtCore.QObject.connect(self.PosIsoEnthLabel,QtCore.SIGNAL("editingFinished()"),self.IsoentalpicaCritica_togggled)
        QtCore.QObject.connect(self.PosIsoEntrLabel,QtCore.SIGNAL("editingFinished()"),self.IsoentropicaCritica_togggled)
        QtCore.QObject.connect(self.PosIsoVolLabel,QtCore.SIGNAL("editingFinished()"),self.IsocoraCritica_togggled)
        QtCore.QObject.connect(self.PosIsoXLabel,QtCore.SIGNAL("editingFinished()"),self.IsoXCritica_togggled)

        QtCore.QObject.connect(self.action2D,QtCore.SIGNAL("toggled(bool)"),self.d2)
        QtCore.QObject.connect(self.action3D,QtCore.SIGNAL("toggled(bool)"),self.d3)
        QtCore.QObject.connect(self.conf_presion,QtCore.SIGNAL("currentIndexChanged(int)"),self.actualizarConfiguracionUnidades)
        QtCore.QObject.connect(self.conf_capacidad,QtCore.SIGNAL("currentIndexChanged(int)"),self.actualizarConfiguracionUnidades)
        QtCore.QObject.connect(self.conf_conductividad,QtCore.SIGNAL("currentIndexChanged(int)"),self.actualizarConfiguracionUnidades)
        QtCore.QObject.connect(self.conf_densidad,QtCore.SIGNAL("currentIndexChanged(int)"),self.actualizarConfiguracionUnidades)
        QtCore.QObject.connect(self.conf_entalpia,QtCore.SIGNAL("currentIndexChanged(int)"),self.actualizarConfiguracionUnidades)
        QtCore.QObject.connect(self.conf_entropia,QtCore.SIGNAL("currentIndexChanged(int)"),self.actualizarConfiguracionUnidades)
        QtCore.QObject.connect(self.conf_temperatura,QtCore.SIGNAL("currentIndexChanged(int)"),self.actualizarConfiguracionUnidades)
        QtCore.QObject.connect(self.conf_velocidad,QtCore.SIGNAL("currentIndexChanged(int)"),self.actualizarConfiguracionUnidades)
        QtCore.QObject.connect(self.conf_viscosidad,QtCore.SIGNAL("currentIndexChanged(int)"),self.actualizarConfiguracionUnidades)
        QtCore.QObject.connect(self.conf_volumen,QtCore.SIGNAL("currentIndexChanged(int)"),self.actualizarConfiguracionUnidades)
        QtCore.QObject.connect(self.English,QtCore.SIGNAL("toggled(bool)"), self.english)
        QtCore.QObject.connect(self.SI,QtCore.SIGNAL("toggled(bool)"), self.si)
        SteamTables.setTabOrder(self.presion, self.temperatura)
        SteamTables.setTabOrder(self.temperatura, self.volumen)
        SteamTables.setTabOrder(self.volumen, self.entalpia)
        SteamTables.setTabOrder(self.entalpia, self.entropia)
        SteamTables.setTabOrder(self.entropia, self.fraccionVapor)
    
    def retranslateUi(self, SteamTables):
        self.menuGrafico.setTitle(QtGui.QApplication.translate("SteamTables", "Gráfico", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMostrarBarra.setText(QtGui.QApplication.translate("SteamTables", "Mostrar barra de herramientas", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSalir.setText(QtGui.QApplication.translate("SteamTables", "Salir", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDibujarSaturacion.setText(QtGui.QApplication.translate("SteamTables", "Dibujar línea de saturación", None, QtGui.QApplication.UnicodeUTF8))
        self.action2D.setText(QtGui.QApplication.translate("SteamTables", "Gráfico 2D", None, QtGui.QApplication.UnicodeUTF8))
        self.action3D.setText(QtGui.QApplication.translate("SteamTables", "Gráfico 3D", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMostrarPuntos.setText(QtGui.QApplication.translate("SteamTables", "Mostrar puntos definidos por el usuario", None, QtGui.QApplication.UnicodeUTF8))
        SteamTables.setWindowTitle(QtGui.QApplication.translate("SteamTables", "Tablas de Vapor", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("SteamTables", "Ejes", None, QtGui.QApplication.UnicodeUTF8))
        self.label_1.setText(QtGui.QApplication.translate("SteamTables", "Datos conocidos", None, QtGui.QApplication.UnicodeUTF8))
        self.ejesTabla.setToolTip(QtGui.QApplication.translate("SteamTables", "p\tPresión\nT\tTemperatura\nh\tEntalpía\ns\tEntropía\nv\tVolumen específico\nx\tCalidad (cuando es vapor saturado)", None, QtGui.QApplication.UnicodeUTF8))
        self.ejesTabla.setStatusTip(QtGui.QApplication.translate("SteamTables", "Definir variables impuestas", None, QtGui.QApplication.UnicodeUTF8))
        self.variablesCalculo.setToolTip(QtGui.QApplication.translate("SteamTables", "p\tPresión\nT\tTemperatura\nh\tEntalpía\ns\tEntropía\nv\tVolumen específico\nx\tCalidad (cuando es vapor saturado)", None, QtGui.QApplication.UnicodeUTF8))
        self.variablesCalculo.setStatusTip(QtGui.QApplication.translate("SteamTables", "Definir variables impuestas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoEntr.setText(QtGui.QApplication.translate("SteamTables", "Isoentrópicas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoEntr.setStatusTip(QtGui.QApplication.translate("SteamTables", "Dibujar curvas isoentrópicas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoEnth.setText(QtGui.QApplication.translate("SteamTables", "Isoentálpicas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoEnth.setStatusTip(QtGui.QApplication.translate("SteamTables", "Dibujar curvas isoentálpicas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoBar.setText(QtGui.QApplication.translate("SteamTables", "Isobáricas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoBar.setStatusTip(QtGui.QApplication.translate("SteamTables", "Dibujar curvas isobáricas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoTherm.setText(QtGui.QApplication.translate("SteamTables", "Isotérmicas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoTherm.setStatusTip(QtGui.QApplication.translate("SteamTables", "Dibujar curvas isotérmicas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoVol.setText(QtGui.QApplication.translate("SteamTables", "Isocóricas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoVol.setStatusTip(QtGui.QApplication.translate("SteamTables", "Dibujar curvas isocóricas", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoX.setText(QtGui.QApplication.translate("SteamTables", "Isocalidad", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoX.setStatusTip(QtGui.QApplication.translate("SteamTables", "Dibujar curvas con igual fracción de vapor", None, QtGui.QApplication.UnicodeUTF8))
        self.label_17.setText(QtGui.QApplication.translate("SteamTables", "Intervalo", None, QtGui.QApplication.UnicodeUTF8))
        self.label_15.setText(QtGui.QApplication.translate("SteamTables", "Inicio", None, QtGui.QApplication.UnicodeUTF8))
        self.label_16.setText(QtGui.QApplication.translate("SteamTables", "Fin", None, QtGui.QApplication.UnicodeUTF8))
        self.botonCalcular.setText(QtGui.QApplication.translate("SteamTables", "Calcular", None, QtGui.QApplication.UnicodeUTF8))
        self.botonActualizar.setText(QtGui.QApplication.translate("SteamTables", "Actualizar", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBox.setItemText(self.toolBox.indexOf(self.page_1), QtGui.QApplication.translate("SteamTables", "Tablas", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBox.setItemText(self.toolBox.indexOf(self.page_Plot), QtGui.QApplication.translate("SteamTables", "Gráfico", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setText(QtGui.QApplication.translate("SteamTables", "Energía interna", None, QtGui.QApplication.UnicodeUTF8))
        self.label_18.setText(QtGui.QApplication.translate("SteamTables", "E. Gibbs", None, QtGui.QApplication.UnicodeUTF8))
        self.label_19.setText(QtGui.QApplication.translate("SteamTables", "E. Helmholtz", None, QtGui.QApplication.UnicodeUTF8))
        self.energiaGibbs.setStatusTip(QtGui.QApplication.translate("SteamTables", "Energía de Gibbs", None, QtGui.QApplication.UnicodeUTF8))
        self.energiaHelmholtz.setStatusTip(QtGui.QApplication.translate("SteamTables", "Energía de Helmholtz", None, QtGui.QApplication.UnicodeUTF8))
        self.label_20.setText(QtGui.QApplication.translate("SteamTables", "Densidad", None, QtGui.QApplication.UnicodeUTF8))
        self.presion.setStatusTip(QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8))
        self.temperatura.setStatusTip(QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8))
        self.label_8.setText(QtGui.QApplication.translate("SteamTables", "Volumen", None, QtGui.QApplication.UnicodeUTF8))
        self.volumen.setStatusTip(QtGui.QApplication.translate("SteamTables", "Volumen específico", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("SteamTables", "Entalpia", None, QtGui.QApplication.UnicodeUTF8))
        self.entalpia.setStatusTip(QtGui.QApplication.translate("SteamTables", "Entalpía", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("SteamTables", "Entropía", None, QtGui.QApplication.UnicodeUTF8))
        self.entropia.setStatusTip(QtGui.QApplication.translate("SteamTables", "Entropia", None, QtGui.QApplication.UnicodeUTF8))
        self.label_12.setText(QtGui.QApplication.translate("SteamTables", "Fracción de vapor", None, QtGui.QApplication.UnicodeUTF8))
        self.fraccionVapor.setStatusTip(QtGui.QApplication.translate("SteamTables", "Fracción de vapor", None, QtGui.QApplication.UnicodeUTF8))
        self.label_14.setText(QtGui.QApplication.translate("SteamTables", "Calcular", None, QtGui.QApplication.UnicodeUTF8))
        self.variableTabla.setStatusTip(QtGui.QApplication.translate("SteamTables", "Definir variables a calcular", None, QtGui.QApplication.UnicodeUTF8))
        self.menuArchivo.setTitle(QtGui.QApplication.translate("SteamTables", "Archivo", None, QtGui.QApplication.UnicodeUTF8))
        self.menuAyuda.setTitle(QtGui.QApplication.translate("SteamTables", "Ayuda", None, QtGui.QApplication.UnicodeUTF8))
        self.actionCSV.setText(QtGui.QApplication.translate("SteamTables", "Guardar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAcerca_de.setText(QtGui.QApplication.translate("SteamTables", "Acerca de freesteam", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAcerca_deQt.setText(QtGui.QApplication.translate("SteamTables", "Acerca de Qt", None, QtGui.QApplication.UnicodeUTF8))
        self.label_21.setText(QtGui.QApplication.translate("SteamTables", "Cp", None, QtGui.QApplication.UnicodeUTF8))
        self.label_22.setText(QtGui.QApplication.translate("SteamTables", "Cv", None, QtGui.QApplication.UnicodeUTF8))
        self.label_23.setText(QtGui.QApplication.translate("SteamTables", "Conductividad", None, QtGui.QApplication.UnicodeUTF8))
        self.label_24.setText(QtGui.QApplication.translate("SteamTables", u"Viscosidad", None, QtGui.QApplication.UnicodeUTF8))
        self.label_25.setText(QtGui.QApplication.translate("SteamTables", "V sonido", None, QtGui.QApplication.UnicodeUTF8))
        self.label_30.setText(QtGui.QApplication.translate("SteamTables", "Región", None, QtGui.QApplication.UnicodeUTF8))
        self.botonAdd.setText(QtGui.QApplication.translate("SteamTables", "Añadir", None, QtGui.QApplication.UnicodeUTF8))
        self.botonAdd.setStatusTip(QtGui.QApplication.translate("SteamTables", "Añadir a la lista de puntos representados en la gráfica", None, QtGui.QApplication.UnicodeUTF8))
        self.botonLista.setText(QtGui.QApplication.translate("SteamTables", "Lista", None, QtGui.QApplication.UnicodeUTF8))
        self.botonLista.setStatusTip(QtGui.QApplication.translate("SteamTables", "Mostrar lista de puntos específicos representados en la gráfica", None, QtGui.QApplication.UnicodeUTF8))
        self.dockWidget_Tabla.setWindowTitle(QtGui.QApplication.translate("SteamTables", "Tabla", None, QtGui.QApplication.UnicodeUTF8))
        self.dockWidget_Isolineas.setWindowTitle(QtGui.QApplication.translate("SteamTables", "Isolíneas", None, QtGui.QApplication.UnicodeUTF8))
        self.dockWidget_Puntos.setWindowTitle(QtGui.QApplication.translate("SteamTables", "Puntos específicos", None, QtGui.QApplication.UnicodeUTF8))
        self.dockWidget_Units.setWindowTitle(QtGui.QApplication.translate("SteamTables", "Unidades", None, QtGui.QApplication.UnicodeUTF8))
        self.dockWidget_2D.setWindowTitle(QtGui.QApplication.translate("SteamTables", "Gráfico 2D", None, QtGui.QApplication.UnicodeUTF8))
        self.energiaInterna.setStatusTip(QtGui.QApplication.translate("SteamTables", "Energía Interna", None, QtGui.QApplication.UnicodeUTF8))
        self.diagrama3D.setStatusTip(QtGui.QApplication.translate("SteamTables", "Pinchar y arrastrar para mover la orientación del gráfico", None, QtGui.QApplication.UnicodeUTF8))
        self.densidad.setStatusTip(QtGui.QApplication.translate("SteamTables", "Densidad", None, QtGui.QApplication.UnicodeUTF8))
        self.cp.setStatusTip(QtGui.QApplication.translate("SteamTables", "Capacidad calorífica a presión constante", None, QtGui.QApplication.UnicodeUTF8))
        self.cv.setStatusTip(QtGui.QApplication.translate("SteamTables", "Capacidad calorífica a volumen constante", None, QtGui.QApplication.UnicodeUTF8))
        self.conductividad.setStatusTip(QtGui.QApplication.translate("SteamTables", "Conductividad térmica", None, QtGui.QApplication.UnicodeUTF8))
        self.viscosidad.setStatusTip(QtGui.QApplication.translate("SteamTables", "Viscosidad dinámica", None, QtGui.QApplication.UnicodeUTF8))
        self.velocidad.setStatusTip(QtGui.QApplication.translate("SteamTables", "Velocidad del sonido", None, QtGui.QApplication.UnicodeUTF8))
        self.label1.setText(QtGui.QApplication.translate("SteamTables", "Inicio", None, QtGui.QApplication.UnicodeUTF8))
        self.label2.setText(QtGui.QApplication.translate("SteamTables", "Fin", None, QtGui.QApplication.UnicodeUTF8))
        self.label3.setText(QtGui.QApplication.translate("SteamTables", "Intervalo", None, QtGui.QApplication.UnicodeUTF8))
        self.PersonalizarIsoterma.setText(QtGui.QApplication.translate("SteamTables", "Personalizar", None, QtGui.QApplication.UnicodeUTF8))
        self.IsotermaCritica.setText(QtGui.QApplication.translate("SteamTables", "Incluir linea del punto crítico", None, QtGui.QApplication.UnicodeUTF8))
        self.Isolineas.setTabText(self.Isolineas.indexOf(self.page), QtGui.QApplication.translate("SteamTables", "Isoterma", None, QtGui.QApplication.UnicodeUTF8))
        self.label1_2.setText(QtGui.QApplication.translate("SteamTables", "Inicio", None, QtGui.QApplication.UnicodeUTF8))
        self.label2_2.setText(QtGui.QApplication.translate("SteamTables", "Fin", None, QtGui.QApplication.UnicodeUTF8))
        self.label3_2.setText(QtGui.QApplication.translate("SteamTables", "Intervalo", None, QtGui.QApplication.UnicodeUTF8))
        self.PersonalizarIsobara.setText(QtGui.QApplication.translate("SteamTables", "Personalizar", None, QtGui.QApplication.UnicodeUTF8))
        self.IsobaraCritica.setText(QtGui.QApplication.translate("SteamTables", "Incluir linea del punto crítico", None, QtGui.QApplication.UnicodeUTF8))
        self.Isolineas.setTabText(self.Isolineas.indexOf(self.page_2), QtGui.QApplication.translate("SteamTables", "Isobara", None, QtGui.QApplication.UnicodeUTF8))
        self.label1_3.setText(QtGui.QApplication.translate("SteamTables", "Inicio", None, QtGui.QApplication.UnicodeUTF8))
        self.label2_3.setText(QtGui.QApplication.translate("SteamTables", "Fin", None, QtGui.QApplication.UnicodeUTF8))
        self.label3_3.setText(QtGui.QApplication.translate("SteamTables", "Intervalo", None, QtGui.QApplication.UnicodeUTF8))
        self.PersonalizarIsoentalpica.setText(QtGui.QApplication.translate("SteamTables", "Personalizar", None, QtGui.QApplication.UnicodeUTF8))
        self.IsoentalpicaCritica.setText(QtGui.QApplication.translate("SteamTables", "Incluir linea del punto crítico", None, QtGui.QApplication.UnicodeUTF8))
        self.Isolineas.setTabText(self.Isolineas.indexOf(self.page_3), QtGui.QApplication.translate("SteamTables", "Isoentálpica", None, QtGui.QApplication.UnicodeUTF8))
        self.label1_4.setText(QtGui.QApplication.translate("SteamTables", "Inicio", None, QtGui.QApplication.UnicodeUTF8))
        self.label2_4.setText(QtGui.QApplication.translate("SteamTables", "Fin", None, QtGui.QApplication.UnicodeUTF8))
        self.label3_4.setText(QtGui.QApplication.translate("SteamTables", "Intervalo", None, QtGui.QApplication.UnicodeUTF8))
        self.PersonalizarIsoentropica.setText(QtGui.QApplication.translate("SteamTables", "Personalizar", None, QtGui.QApplication.UnicodeUTF8))
        self.IsoentropicaCritica.setText(QtGui.QApplication.translate("SteamTables", "Incluir linea del punto crítico", None, QtGui.QApplication.UnicodeUTF8))
        self.Isolineas.setTabText(self.Isolineas.indexOf(self.page_4), QtGui.QApplication.translate("SteamTables", "Isoentrópica", None, QtGui.QApplication.UnicodeUTF8))
        self.label1_5.setText(QtGui.QApplication.translate("SteamTables", "Inicio", None, QtGui.QApplication.UnicodeUTF8))
        self.label2_5.setText(QtGui.QApplication.translate("SteamTables", "Fin", None, QtGui.QApplication.UnicodeUTF8))
        self.label3_5.setText(QtGui.QApplication.translate("SteamTables", "Intervalo", None, QtGui.QApplication.UnicodeUTF8))
        self.PersonalizarIsocora.setText(QtGui.QApplication.translate("SteamTables", "Personalizar", None, QtGui.QApplication.UnicodeUTF8))
        self.IsocoraCritica.setText(QtGui.QApplication.translate("SteamTables", "Incluir linea del punto crítico", None, QtGui.QApplication.UnicodeUTF8))
        self.Isolineas.setTabText(self.Isolineas.indexOf(self.page_5), QtGui.QApplication.translate("SteamTables", "Isocora", None, QtGui.QApplication.UnicodeUTF8))
        self.label1_6.setText(QtGui.QApplication.translate("SteamTables", "Inicio", None, QtGui.QApplication.UnicodeUTF8))
        self.label2_6.setText(QtGui.QApplication.translate("SteamTables", "Fin", None, QtGui.QApplication.UnicodeUTF8))
        self.label3_6.setText(QtGui.QApplication.translate("SteamTables", "Intervalo", None, QtGui.QApplication.UnicodeUTF8))
        self.PersonalizarIsoX.setText(QtGui.QApplication.translate("SteamTables", "Personalizar", None, QtGui.QApplication.UnicodeUTF8))
        self.Isolineas.setTabText(self.Isolineas.indexOf(self.page_6), QtGui.QApplication.translate("SteamTables", "Isocalidad", None, QtGui.QApplication.UnicodeUTF8))
        self.tabEjes.setTabText(self.tabEjes.indexOf(self.groupBox_3), QtGui.QApplication.translate("SteamTables", "General", None, QtGui.QApplication.UnicodeUTF8))
        self.tabEjes.setTabText(self.tabEjes.indexOf(self.groupBox_1), QtGui.QApplication.translate("SteamTables", "Ejes", None, QtGui.QApplication.UnicodeUTF8))
        self.tabEjes.setTabText(self.tabEjes.indexOf(self.groupBox_2), QtGui.QApplication.translate("SteamTables", "Etiquetas", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("SteamTables", "Sistema de unidades", None, QtGui.QApplication.UnicodeUTF8))
        self.SI.setText(QtGui.QApplication.translate("SteamTables", "SI", None, QtGui.QApplication.UnicodeUTF8))
        self.English.setText(QtGui.QApplication.translate("SteamTables", "Inglés", None, QtGui.QApplication.UnicodeUTF8))
        self.ejeX_escala.setText(QtGui.QApplication.translate("SteamTables", "Escala logarítmica", None, QtGui.QApplication.UnicodeUTF8))
        self.ejeY_escala.setText(QtGui.QApplication.translate("SteamTables", "Escala logarítmica", None, QtGui.QApplication.UnicodeUTF8))
        self.rejilla.setText(QtGui.QApplication.translate("SteamTables", "Rejilla", None, QtGui.QApplication.UnicodeUTF8))
        self.label_28.setText(QtGui.QApplication.translate("SteamTables", "Variable", None, QtGui.QApplication.UnicodeUTF8))
        self.label_29.setText(QtGui.QApplication.translate("SteamTables", "Variable", None, QtGui.QApplication.UnicodeUTF8))
        self.label_31.setText(QtGui.QApplication.translate("SteamTables", "Estilo de línea", None, QtGui.QApplication.UnicodeUTF8))
        self.label_32.setText(QtGui.QApplication.translate("SteamTables", "Estilo de línea", None, QtGui.QApplication.UnicodeUTF8))
        self.label_33.setText(QtGui.QApplication.translate("SteamTables", "Estilo de línea", None, QtGui.QApplication.UnicodeUTF8))
        self.label_34.setText(QtGui.QApplication.translate("SteamTables", "Estilo de línea", None, QtGui.QApplication.UnicodeUTF8))
        self.label_35.setText(QtGui.QApplication.translate("SteamTables", "Estilo de línea", None, QtGui.QApplication.UnicodeUTF8))
        self.label_40.setText(QtGui.QApplication.translate("SteamTables", "Estilo de línea", None, QtGui.QApplication.UnicodeUTF8))
        self.label_36.setText(QtGui.QApplication.translate("SteamTables", "Mínimo", None, QtGui.QApplication.UnicodeUTF8))
        self.label_37.setText(QtGui.QApplication.translate("SteamTables", "Máximo", None, QtGui.QApplication.UnicodeUTF8))
        self.label_38.setText(QtGui.QApplication.translate("SteamTables", "Mínimo", None, QtGui.QApplication.UnicodeUTF8))
        self.label_39.setText(QtGui.QApplication.translate("SteamTables", "Máximo", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoThermLabel.setText(QtGui.QApplication.translate("SteamTables", "Isoterma", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoBarLabel.setText(QtGui.QApplication.translate("SteamTables", "Isobara", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoEnthLabel.setText(QtGui.QApplication.translate("SteamTables", "Isoentálpica", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoEntrLabel.setText(QtGui.QApplication.translate("SteamTables", "Isoentrópica", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoVolLabel.setText(QtGui.QApplication.translate("SteamTables", "Isocora", None, QtGui.QApplication.UnicodeUTF8))
        self.checkIsoXLabel.setText(QtGui.QApplication.translate("SteamTables", "Isocalidad", None, QtGui.QApplication.UnicodeUTF8))
        self.label_41.setText(QtGui.QApplication.translate("SteamTables", "Posición", None, QtGui.QApplication.UnicodeUTF8))
        self.label_42.setText(QtGui.QApplication.translate("SteamTables", "Posición", None, QtGui.QApplication.UnicodeUTF8))
        self.label_43.setText(QtGui.QApplication.translate("SteamTables", "Posición", None, QtGui.QApplication.UnicodeUTF8))
        self.label_44.setText(QtGui.QApplication.translate("SteamTables", "Posición", None, QtGui.QApplication.UnicodeUTF8))
        self.label_45.setText(QtGui.QApplication.translate("SteamTables", "Posición", None, QtGui.QApplication.UnicodeUTF8))
        self.label_46.setText(QtGui.QApplication.translate("SteamTables", "Posición", None, QtGui.QApplication.UnicodeUTF8))
          
    def acerca(self):
        QtGui.QMessageBox.information(SteamTables,"Acerca de" ,"""<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">\n<html><head><meta name="qrichtext" content="1" /><style type="text/css">\np, li { white-space: pre-wrap; }\n</style></head><body style=" font-family:'Nimbus Sans L'; font-size:9pt; font-weight:400; font-style:normal;">\n<table border="0" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px;" cellspacing="2" cellpadding="0">\n<tr>\n<td style=" vertical-align:top;">\n<p style=" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-weight:600;">freesteam</span> is an open source implementation of international-standard IAPWS-IF97 steam tables from the <a href="http://www.iapws.org"><span style=" text-decoration: underline; color:#0000ff;">International Association for the Properties of Water and Steam</span></a> (IAPWS). <span style=" font-weight:600;">freesteam</span> lets you compute water and steam properties for a wide range of pressures and temperatures: you can specify the state of the steam in terms of a variety of combinations of 'known' properties, then freesteam will solve and allow you to query to find the values of the 'unknown' properties.</p>\n<p style=" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">Website: <a href="http://freesteam.sourceforge.net/"><span style=" text-decoration: underline; color:#0000ff;">http://freesteam.sourceforge.net/</span></a></p></td></tr></table></body></html>""")
        
    def acercaQt(self):
        QtGui.QMessageBox.aboutQt(SteamTables,"Acerca de Qt" )

    def exporttoCSV(self):
        """Guarda los datos de la tabla en un archivo csv"""
        fname = QtGui.QFileDialog.getSaveFileName(SteamTables, QtGui.QApplication.translate("SteamTables", u"Exportar datos", None, QtGui.QApplication.UnicodeUTF8), "./", "CSV (*.csv);;All archives (*.*)")
        if fname:
            texto = writer(open(fname, 'wb'), delimiter='\t')
            texto.writerow([""]+[str(i) for i in self.xdata[0]])
            for i, fila in enumerate(self.zdata):
                texto.writerow([str(self.ydata[i][0])]+[str(i) for i in fila])

    def closeEvent(self, event):
        """Guarda la configuración antes de salir"""
        Config=ConfigParser()
        Config.read("UI_steamTablesrc")
        if not Config.has_section("Table"):
            Config.add_section("Table")
        Config.set("Table", "Axis", self.ejesTabla.currentIndex())
        Config.set("Table", "Calculate", self.variableTabla.currentIndex())
        Config.set("Table", "x_start", self.abscisaInicio.text())
        Config.set("Table", "x_end", self.abscisaFin.text())
        Config.set("Table", "x_step", self.abscisaIntervalo.text())
        Config.set("Table", "y_start", self.ordenadaInicio.text())
        Config.set("Table", "y_end", self.ordenadaFin.text())
        Config.set("Table", "y_step", self.ordenadaIntervalo.text())
        if not Config.has_section("General"):
            Config.add_section("General")
        Config.set("General", "Sat", self.actionDibujarSaturacion.isChecked())
        Config.set("General", "Points", self.actionMostrarPuntos.isChecked())
        Config.set("General", "Toolbar", self.actionMostrarBarra.isChecked())
        Config.set("General", "2D", self.action2D.isChecked())
        Config.set("General", "Plot", self.page_Plot.isVisible())
        Config.set("General", "Isotherm", self.checkIsoTherm.isChecked())
        Config.set("General", "Isobar", self.checkIsoBar.isChecked())
        Config.set("General", "Isoenthalpic", self.checkIsoEnth.isChecked())
        Config.set("General", "Isoentropic", self.checkIsoEntr.isChecked())
        Config.set("General", "Isochor", self.checkIsoVol.isChecked())
        Config.set("General", "Isoquality", self.checkIsoX.isChecked())
        if not Config.has_section("2D"):
            Config.add_section("2D")
        Config.set("2D", "Xvariable", self.ejeX.currentText())
        Config.set("2D", "XScale", self.ejeX_escala.isChecked())
        Config.set("2D", "XMin", self.ejeX_min.text())
        Config.set("2D", "XMax", self.ejeX_max.text())
        Config.set("2D", "Yvariable", self.ejeY.currentText())
        Config.set("2D", "YScale", self.ejeY_escala.isChecked())
        Config.set("2D", "YMin", self.ejeY_min.text())
        Config.set("2D", "YMax", self.ejeY_max.text())
        Config.set("2D", "Grid", self.rejilla.isChecked())
        Config.set("2D", "IsothermLabel", self.checkIsoThermLabel.isChecked())
        Config.set("2D", "IsothermLabelPos", self.PosIsoThermLabel.value())
        Config.set("2D", "IsobarLabel", self.checkIsoBarLabel.isChecked())
        Config.set("2D", "IsobarLabelPos", self.PosIsoBarLabel.value())
        Config.set("2D", "IsoenthLabel", self.checkIsoEnthLabel.isChecked())
        Config.set("2D", "IsoenthLabelPos", self.PosIsoEnthLabel.value())
        Config.set("2D", "IsoentrLabel", self.checkIsoEntrLabel.isChecked())
        Config.set("2D", "IsoentrLabelPos", self.PosIsoEntrLabel.value())
        Config.set("2D", "IsochorLabel", self.checkIsoVolLabel.isChecked())
        Config.set("2D", "IsochorLabelPos", self.PosIsoVolLabel.value())
        Config.set("2D", "IsoXLabel", self.checkIsoXLabel.isChecked())
        Config.set("2D", "IsoXLabelPos", self.PosIsoXLabel.value())
        if not Config.has_section("Isotherm"):
            Config.add_section("Isotherm")
        Config.set("Isotherm", "Start", unidades.Temperature(self.T_ini.text()).unit(config.Configuracion("Temperature").func()))
        Config.set("Isotherm", "End", unidades.Temperature(self.T_fin.text()).unit(config.Configuracion("Temperature").func()))
        Config.set("Isotherm", "Step", unidades.DeltaT(self.T_intervalo.text()).unit(config.Configuracion("Temperature").func()))
        Config.set("Isotherm", "Custom", int(self.PersonalizarIsoterma.isChecked()))
        T=[]
        if not self.ListaIsoterma.text().isEmpty():
            T1=self.ListaIsoterma.text().split(',')
            for i in T1:
                T.append(config.representacion(unidades.Temperature(float(i)).unit(config.Configuracion("Temperature").func())))
        Config.set("Isotherm", "List", ",".join(T))
        Config.set("Isotherm", "Critic", int(self.IsotermaCritica.isChecked()))
        Config.set("Isotherm", "Color", self.IsotermaColor.name())
        Config.set("Isotherm", "lineWidth", self.IsotermaGrosor.value())
        Config.set("Isotherm", "lineStyle", self.IsotermaLinea.currentIndex())
        if not Config.has_section("Isobar"):
            Config.add_section("Isobar")
        Config.set("Isobar", "Start", unidades.Pressure(self.P_ini.text()).unit(config.Configuracion("Pressure").func()))
        Config.set("Isobar", "End", unidades.Pressure(self.P_fin.text()).unit(config.Configuracion("Pressure").func()))
        Config.set("Isobar", "Step", unidades.Pressure(self.P_intervalo.text()).unit(config.Configuracion("Pressure").func()))
        Config.set("Isobar", "Custom", self.PersonalizarIsobara.isChecked())
        P=[]
        if not self.ListaIsobara.text().isEmpty():
            P1=self.ListaIsobara.text().split(',')
            for i in P1:
                P.append(config.representacion(unidades.Pressure(float(i)).unit(config.Configuracion("Pressure").func())))
            Config.set("Isobar", "List", ",".join(P))
        Config.set("Isobar", "Critic", self.IsobaraCritica.isChecked())
        Config.set("Isobar", "Color", self.IsobaraColor.name())
        Config.set("Isobar", "lineWidth", self.IsobaraGrosor.value())
        Config.set("Isobar", "lineStyle", self.IsobaraLinea.currentIndex())
        if not Config.has_section("Isochor"):
            Config.add_section("Isochor")
        Config.set("Isochor", "Start", unidades.SpecificVolume(self.V_ini.text()).unit(config.Configuracion("SpecificVolume").func()))
        Config.set("Isochor", "End", unidades.SpecificVolume(self.V_fin.text()).unit(config.Configuracion("SpecificVolume").func()))
        Config.set("Isochor", "Step", unidades.SpecificVolume(self.V_intervalo.text()).unit(config.Configuracion("SpecificVolume").func()))
        Config.set("Isochor", "Custom", int(self.PersonalizarIsocora.isChecked()))
        V=[]
        if not self.ListaIsocora.text().isEmpty():
            V1=self.ListaIsocora.text().split(',')
            for i in V1:
                V.append(config.representacion(unidades.SpecificVolume(float(i)).unit(config.Configuracion("SpecificVolume").func())))
        Config.set("Isochor", "List", ",".join(V))
        Config.set("Isochor", "Critic", self.IsocoraCritica.isChecked())
        Config.set("Isochor", "Color", self.IsocoraColor.name())
        Config.set("Isochor", "lineWidth", self.IsocoraGrosor.value())
        Config.set("Isochor", "lineStyle", self.IsocoraLinea.currentIndex())
        if not Config.has_section("Isoquality"):
            Config.add_section("Isoquality")
        Config.set("Isoquality", "Start", self.X_ini.text())
        Config.set("Isoquality", "End", self.X_fin.text())
        Config.set("Isoquality", "Step", self.X_intervalo.text())
        Config.set("Isoquality", "Custom", self.PersonalizarIsoX.isChecked())
        Config.set("Isoquality", "List", self.ListaIsoX.text())
        Config.set("Isoquality", "Color", self.IsoXColor.name())
        Config.set("Isoquality", "lineWidth", self.IsoXGrosor.value())
        Config.set("Isoquality", "lineStyle", self.IsoXLinea.currentIndex())
        if not Config.has_section("Isoenthalpic"):
            Config.add_section("Isoenthalpic")
        Config.set("Isoenthalpic", "Start", unidades.Enthalpy(self.H_ini.text()).unit(config.Configuracion("Enthalpy").func()))
        Config.set("Isoenthalpic", "End", unidades.Enthalpy(self.H_fin.text()).unit(config.Configuracion("Enthalpy").func()))
        Config.set("Isoenthalpic", "Step", unidades.Enthalpy(self.H_intervalo.text()).unit(config.Configuracion("Enthalpy").func()))
        Config.set("Isoenthalpic", "Custom", int(self.PersonalizarIsoentalpica.isChecked()))
        H=[]
        if not self.ListaIsoentalpica.text().isEmpty():
            H1=self.ListaIsoentalpica.text().split(',')
            for i in H1:
                H.append(config.representacion(unidades.Enthalpy(float(i)).unit(config.Configuracion("Enthalpy").func())))
        Config.set("Isoenthalpic", "List", ",".join(H))
        Config.set("Isoenthalpic", "Critic", self.IsoentalpicaCritica.isChecked())
        Config.set("Isoenthalpic", "Color", self.IsoentalpicaColor.name())
        Config.set("Isoenthalpic", "lineWidth", self.IsoentalpicaGrosor.value())
        Config.set("Isoenthalpic", "lineStyle", self.IsoentalpicaLinea.currentIndex())
        if not Config.has_section("Isoentropic"):
            Config.add_section("Isoentropic")
        Config.set("Isoentropic", "Start", unidades.SpecificHeat(self.S_ini.text()).unit(config.Configuracion("SpecificHeat", "Entropy").func()))
        Config.set("Isoentropic", "End", unidades.SpecificHeat(self.S_fin.text()).unit(config.Configuracion("SpecificHeat", "Entropy").func()))
        Config.set("Isoentropic", "Step", unidades.SpecificHeat(self.S_intervalo.text()).unit(config.Configuracion("SpecificHeat", "Entropy").func()))
        Config.set("Isoentropic", "Custom", self.PersonalizarIsoentropica.isChecked())
        S=[]
        if not self.ListaIsoentropica.text().isEmpty():
            S1=self.ListaIsoentropica.text().split(',')
            for i in S1:
                S.append(config.representacion(unidades.SpecificHeat(float(i)).unit(config.Configuracion("SpecificHeat", "Entropy").func())))
        Config.set("Isoentropic", "List", ",".join(S))
        Config.set("Isoentropic", "Critic", int(self.IsoentropicaCritica.isChecked()))
        Config.set("Isoentropic", "Color", self.IsoentropicaColor.name())
        Config.set("Isoentropic", "lineWidth", self.IsoentropicaGrosor.value())
        Config.set("Isoentropic", "lineStyle", self.IsoentropicaLinea.currentIndex())
        if len(self.puntos)>0:
            Config.remove_section("Points")
            Config.add_section("Points")
            for i, punto in enumerate(self.puntos):
                if punto.region=="\x04":
                    Config.set("Points", str(i), "0"+","+str(punto.T)+","+str(punto.x))
                else:
                    Config.set("Points", str(i), "1"+","+str(punto.T)+","+str(punto.p))
            Config.set("Points", "Variable", self.variablesCalculo.currentIndex())
        Config.write(open("UI_steamTablesrc", "w"))
        event.accept()
                
    def actualizarConfiguracionUnidades(self):
        """Cada vez que se cambia algo de las opciones de unidades las guarda y actualiza los valores mostrados"""
        if self.factorx==0 and self.factory==0:
            self.factores_conversion()
        if self.semaforo.available()>0:
            self.semaforo.acquire(1)
            T_ini=unidades.Temperature(self.T_ini.text()).unit(config.Configuracion("Temperature").func())
            T_fin=unidades.Temperature(self.T_fin.text()).unit(config.Configuracion("Temperature").func())
            T_intervalo=unidades.DeltaT(self.T_intervalo.text()).unit(config.Configuracion("Temperature").func())
            P_ini=unidades.Pressure(self.P_ini.text()).unit(config.Configuracion("Pressure").func())
            P_fin=unidades.Pressure(self.P_fin.text()).unit(config.Configuracion("Pressure").func())
            P_intervalo=unidades.Pressure(self.P_intervalo.text()).unit(config.Configuracion("Pressure").func())
            S_ini=unidades.SpecificHeat(self.S_ini.text()).unit(config.Configuracion("SpecificHeat","Entropy").func())
            S_fin=unidades.SpecificHeat(self.S_fin.text()).unit(config.Configuracion("SpecificHeat","Entropy").func())
            S_intervalo=unidades.SpecificHeat(self.S_intervalo.text()).unit(config.Configuracion("SpecificHeat","Entropy").func())
            H_ini=unidades.Enthalpy(self.H_ini.text()).unit(config.Configuracion("Enthalpy").func())
            H_fin=unidades.Enthalpy(self.H_fin.text()).unit(config.Configuracion("Enthalpy").func())
            H_intervalo=unidades.Enthalpy(self.H_intervalo.text()).unit(config.Configuracion("Enthalpy").func())
            V_ini=unidades.SpecificVolume(self.V_ini.text()).unit(config.Configuracion("SpecificVolume").func())
            V_fin=unidades.SpecificVolume(self.V_fin.text()).unit(config.Configuracion("SpecificVolume").func())
            V_intervalo=unidades.SpecificVolume(self.V_intervalo.text()).unit(config.Configuracion("SpecificVolume").func())
            if self.factorx2==0:  #El eje x es la temperatura
                xmax=unidades.Temperature(self.ejeX_max.text()).unit(config.Configuracion("Temperature").func())
                xmin=unidades.Temperature(self.ejeX_min.text()).unit(config.Configuracion("Temperature").func())
            else: #En cualquier otro caso basta con usar el factor de correción para ese eje
                xmax=float(self.ejeX_max.text())*self.factorx2
                xmin=float(self.ejeX_min.text())*self.factorx2
            if self.factory2==0:  
                ymax=unidades.Temperature(self.ejeY_max.text()).unit(config.Configuracion("Temperature").func())
                ymin=unidades.Temperature(self.ejeY_min.text()).unit(config.Configuracion("Temperature").func())
            else: 
                ymax=float(self.ejeY_max.text())*self.factory2
                ymin=float(self.ejeY_min.text())*self.factory2      
                
            T=[]
            if not self.ListaIsoterma.text().isEmpty():
                for i in self.ListaIsoterma.text().split(','):
                    T.append(str(unidades.Temperature(i).unit(config.Configuracion("Temperature").func())))
            P=[]
            if not self.ListaIsobara.text().isEmpty():
                for i in self.ListaIsobara.text().split(','):
                    P.append(str(unidades.Pressure(i).unit(config.Configuracion("Pressure").func())))
            V=[]
            if not self.ListaIsocora.text().isEmpty():
                for i in self.ListaIsocora.text().split(','):
                    V.append(str(unidades.SpecificVolume(i).unit(config.Configuracion("SpecificVolume").func())))
            H=[]
            if not self.ListaIsoentalpica.text().isEmpty():
                for i in self.ListaIsoentalpica.text().split(','):
                    H.append(str(unidades.Enthalpy(i).unit(config.Configuracion("Enthalpy").func())))
            S=[]
            if not self.ListaIsoentropica.text().isEmpty():
                for i in self.ListaIsoentropica.text().split(','):
                    S.append(str(unidades.SpecificHeat(i).unit(config.Configuracion("SpecificHeat","Entropy").func())))
            Config=ConfigParser()
            Config.read("UI_steamTablesrc")
            if not Config.has_section("Units"):
                Config.add_section("Units")
            Config.set("Units", "Temperature", self.conf_temperatura.currentIndex())
            Config.set("Units", "Pressure", self.conf_presion.currentIndex())
            Config.set("Units", "Density", self.conf_densidad.currentIndex())
            Config.set("Units", "ThermalConductivity", self.conf_conductividad.currentIndex())
            Config.set("Units", "SpecificHeat", self.conf_capacidad.currentIndex())
            Config.set("Units", "SpecificVolume", self.conf_volumen.currentIndex())
            Config.set("Units", "Enthalpy", self.conf_entalpia.currentIndex())
            Config.set("Units", "Speed", self.conf_velocidad.currentIndex())
            Config.set("Units", "Viscosity", self.conf_viscosidad.currentIndex())
            Config.set("Units", "Entropy", self.conf_entropia.currentIndex())
            Config.write(open("UI_steamTablesrc", "w"))
            self.mostrarUnidades()
            self.mostrarPropiedades()
            self.T_ini.setText(config.representacion(T_ini.config))
            self.T_fin.setText(config.representacion(T_fin.config))
            self.T_intervalo.setText(config.representacion(T_intervalo.config))
            self.P_ini.setText(config.representacion(P_ini.config))
            self.P_fin.setText(config.representacion(P_fin.config))
            self.P_intervalo.setText(config.representacion(P_intervalo.config))
            self.S_ini.setText(config.representacion(S_ini.config("Entropy")))
            self.S_fin.setText(config.representacion(S_fin.config("Entropy")))
            self.S_intervalo.setText(config.representacion(S_intervalo.config("Entropy")))
            self.H_ini.setText(config.representacion(H_ini.config))
            self.H_fin.setText(config.representacion(H_fin.config))
            self.H_intervalo.setText(config.representacion(H_intervalo.config))
            self.V_ini.setText(config.representacion(V_ini.config))
            self.V_fin.setText(config.representacion(V_fin.config))
            self.V_intervalo.setText(config.representacion(V_intervalo.config))
            self.factores_conversion()
            if self.factorx2==0:  
                self.ejeX_max.setText(config.representacion(xmax.config))
                self.ejeX_min.setText(config.representacion(xmin.config))
            else: 
                self.ejeX_max.setText(config.representacion(xmax/self.factorx2))
                self.ejeX_min.setText(config.representacion(xmin/self.factorx2))
            if self.factory2==0:  
                self.ejeY_max.setText(config.representacion(ymax.config))
                self.ejeY_min.setText(config.representacion(ymin.config))
            else: 
                self.ejeY_max.setText(config.representacion(ymax/self.factory2))
                self.ejeY_min.setText(config.representacion(ymin/self.factory2))
            T1=[]
            for i in T:
                T1.append(config.representacion(unidades.Temperature(i).config))
            self.ListaIsoterma.setText(",".join(T1))
            P1=[]
            for i in P:
                P1.append(config.representacion(unidades.Pressure(i).config))
            self.ListaIsobara.setText(",".join(P1))
            V1=[]
            for i in V:
                V1.append(config.representacion(unidades.SpecificVolume((i)).config))
            self.ListaIsocora.setText(",".join(V1))
            H1=[]
            for i in H:
                H1.append(config.representacion(unidades.Enthalpy(i).config))
            self.ListaIsoentalpica.setText(",".join(H1))
            S1=[]
            for i in S:
                S1.append(config.representacion(unidades.SpecificHeat(i).config("Entropy")))
            self.ListaIsoentropica.setText(",".join(S1))

            self.semaforo.release(1)

    def mostrarUnidades(self):
        """Muestra el texto de las unidades en función de la configuración"""
        self.label_2_2.setText(config.Configuracion("Pressure").text())
        self.label_3_2.setText(config.Configuracion("Temperature").text())
        self.label_8_2.setText(config.Configuracion("SpecificVolume").text())
        self.label_4_2.setText(config.Configuracion("Enthalpy").text())
        self.label_5_2.setText(config.Configuracion("SpecificHeat", "Entropy").text())
        self.label_13_2.setText(config.Configuracion("Enthalpy").text())
        self.label_21_2.setText(config.Configuracion("SpecificHeat").text())
        self.label_19_2.setText(config.Configuracion("Enthalpy").text())
        self.label_18_2.setText(config.Configuracion("Enthalpy").text())
        self.label_22_2.setText(config.Configuracion("SpecificHeat").text())
        self.label_20_2.setText(config.Configuracion("Density").text())
        self.label_23_2.setText(config.Configuracion("ThermalConductivity").text())
        self.label_24_2.setText(config.Configuracion("Viscosity").text())
        self.label_25_2.setText(config.Configuracion("Speed").text())

    def mostrarPropiedades(self):
        """Muestra los valores de las propiedades de la sección de puntos especificados por el usuario"""
        if self.punto<>0:
            self.presion.setText(config.representacion(unidades.Pressure(self.punto.p).config))
            self.temperatura.setText(config.representacion(unidades.Temperature(self.punto.T).config))
            self.volumen.setText(config.representacion(unidades.SpecificVolume(self.punto.v).config))
            self.entalpia.setText(config.representacion(unidades.Enthalpy(self.punto.h).config))
            self.entropia.setText(config.representacion(unidades.SpecificHeat(self.punto.s).config("Entropy")))
            self.energiaInterna.setText(config.representacion(unidades.Enthalpy(self.punto.u).config))
            self.energiaGibbs.setText(config.representacion(unidades.Enthalpy(self.punto.h-self.punto.T*self.punto.s).config))
            self.energiaHelmholtz.setText(config.representacion(unidades.Enthalpy(self.punto.u-self.punto.T*self.punto.s).config))
            self.fraccionVapor.setText(config.representacion(self.punto.x))
            self.densidad.setText(config.representacion(unidades.Density(self.punto.rho).config))
            self.cp.setText(config.representacion(unidades.SpecificHeat(self.punto.cp).config()))
            self.cv.setText(config.representacion(unidades.SpecificHeat(self.punto.cv).config()))
            self.conductividad.setText(config.representacion(unidades.ThermalConductivity(self.punto.k).config))
            self.viscosidad.setText(config.representacion(unidades.Viscosity(self.punto.mu).config))
            if self.punto.region !='\x04' and  self.punto.region !='\x03':
                self.velocidad.setText(config.representacion(unidades.Speed(self.punto.w).config))
            else:
                self.velocidad.setText("nan")
            if self.punto.region =='\x01':
                self.region.setText("1")
            elif self.punto.region =='\x02':
                self.region.setText("2")
            elif self.punto.region =='\x03':
                self.region.setText("3")
            elif self.punto.region =='\x04':
                self.region.setText("4")
            elif self.punto.region =='\x05':
                self.region.setText("5")

    def variablesCalculo_currentIndexChanged(self, indice):
        """Realiza las diferentes acciones necesarias cuando se cambian las variables impuestas"""
        activo = QtGui.QPalette()
        activo.setColor(QtGui.QPalette.Base, QtGui.QColor("yellow"))
        inactivo = QtGui.QPalette()
        inactivo.setColor(QtGui.QPalette.Base, QtGui.QColor("white"))
        if indice==0:
            self.presion.setReadOnly(False)
            self.presion.setPalette(activo)
            self.unidadesPresion.setEnabled(True)
            self.temperatura.setReadOnly(False)
            self.temperatura.setPalette(activo)
            self.unidadesTemperatura.setEnabled(True)
            self.entalpia.setReadOnly(True)
            self.entalpia.setPalette(inactivo)
            self.unidadesEntalpia.setEnabled(False)
            self.entropia.setReadOnly(True)
            self.entropia.setPalette(inactivo)
            self.unidadesEntropia.setEnabled(False)
            self.volumen.setReadOnly(True)
            self.volumen.setPalette(inactivo)
            self.unidadesVolumen.setEnabled(False)
            self.fraccionVapor.setReadOnly(True)
            self.fraccionVapor.setPalette(inactivo)
        elif indice==1:
            self.entalpia.setReadOnly(False)
            self.entalpia.setPalette(activo)
            self.unidadesEntalpia.setEnabled(True)
            self.presion.setReadOnly(False)
            self.presion.setPalette(activo)
            self.unidadesPresion.setEnabled(True)
            self.temperatura.setReadOnly(True)
            self.temperatura.setPalette(inactivo)
            self.unidadesTemperatura.setEnabled(False)
            self.entropia.setReadOnly(True)
            self.entropia.setPalette(inactivo)
            self.unidadesEntropia.setEnabled(False)
            self.volumen.setReadOnly(True)
            self.volumen.setPalette(inactivo)
            self.unidadesVolumen.setEnabled(False)
            self.fraccionVapor.setReadOnly(True)
            self.fraccionVapor.setPalette(inactivo)
        elif indice==2:
            self.entropia.setReadOnly(False)
            self.entropia.setPalette(activo)
            self.unidadesEntropia.setEnabled(True)
            self.presion.setReadOnly(False)
            self.presion.setPalette(activo)
            self.unidadesPresion.setEnabled(True)
            self.temperatura.setReadOnly(True)
            self.temperatura.setPalette(inactivo)
            self.unidadesTemperatura.setEnabled(False)
            self.entalpia.setReadOnly(True)
            self.entalpia.setPalette(inactivo)
            self.unidadesEntalpia.setEnabled(False)
            self.volumen.setReadOnly(True)
            self.volumen.setPalette(inactivo)
            self.unidadesVolumen.setEnabled(False)
            self.fraccionVapor.setReadOnly(True)
            self.fraccionVapor.setPalette(inactivo)
        elif indice==3:
            self.volumen.setReadOnly(False)
            self.volumen.setPalette(activo)
            self.unidadesVolumen.setEnabled(True)
            self.presion.setReadOnly(False)
            self.presion.setPalette(activo)
            self.unidadesPresion.setEnabled(True)
            self.temperatura.setReadOnly(True)
            self.temperatura.setPalette(inactivo)
            self.unidadesTemperatura.setEnabled(False)
            self.entropia.setReadOnly(True)
            self.entropia.setPalette(inactivo)
            self.unidadesEntropia.setEnabled(False)
            self.entalpia.setReadOnly(True)
            self.entalpia.setPalette(inactivo)
            self.unidadesEntalpia.setEnabled(False)
            self.fraccionVapor.setReadOnly(True)
            self.fraccionVapor.setPalette(inactivo)
        elif indice==4:
            self.entropia.setReadOnly(False)
            self.entropia.setPalette(activo)
            self.unidadesEntropia.setEnabled(True)
            self.temperatura.setReadOnly(False)
            self.temperatura.setPalette(activo)
            self.unidadesTemperatura.setEnabled(True)
            self.entalpia.setReadOnly(True)
            self.entalpia.setPalette(inactivo)
            self.unidadesEntalpia.setEnabled(False)
            self.presion.setReadOnly(True)
            self.presion.setPalette(inactivo)
            self.unidadesPresion.setEnabled(False)
            self.volumen.setReadOnly(True)
            self.volumen.setPalette(inactivo)
            self.unidadesVolumen.setEnabled(False)
            self.fraccionVapor.setReadOnly(True)
            self.fraccionVapor.setPalette(inactivo)
        elif indice==5:
            self.fraccionVapor.setReadOnly(False)
            self.fraccionVapor.setPalette(activo)
            self.temperatura.setReadOnly(False)
            self.temperatura.setPalette(activo)
            self.unidadesTemperatura.setEnabled(True)
            self.entalpia.setReadOnly(True)
            self.entalpia.setPalette(inactivo)
            self.unidadesEntalpia.setEnabled(False)
            self.presion.setReadOnly(True)
            self.presion.setPalette(inactivo)
            self.unidadesPresion.setEnabled(False)
            self.volumen.setReadOnly(True)
            self.volumen.setPalette(inactivo)
            self.unidadesVolumen.setEnabled(False)
            self.entropia.setReadOnly(True)
            self.entropia.setPalette(inactivo)
            self.unidadesEntropia.setEnabled(False)


    def presion_editingFinished(self):
        valido=True
        if len(self.presion.text())>0:
            try:
                self.presion.setText(config.representacion(float(self.presion.text().replace(',', '.'))))
            except ValueError:
                self.presion.setText("")
                valido=False
        if self.variablesCalculo.currentIndex()==0:
            otro=self.temperatura
        elif self.variablesCalculo.currentIndex()==1:
            otro=self.entalpia
        elif self.variablesCalculo.currentIndex()==2:
            otro=self.entropia
        else:
            otro=self.volumen
        if otro.text()<>"" and valido:
            self.calcularPropiedades()
        
    def temperatura_editingFinished(self):
        valido=True
        if len(self.temperatura.text())>0:
            try:
                self.temperatura.setText(config.representacion(float(self.temperatura.text().replace(',', '.'))))
            except ValueError:
                self.temperatura.setText("")
                valido=False
        if self.variablesCalculo.currentIndex()==0:
            otro=self.presion
        elif self.variablesCalculo.currentIndex()==4:
            otro=self.entropia
        else:
            otro=self.fraccionVapor
        if otro.text()<>""and valido:
            self.calcularPropiedades()
        
    def entalpia_editingFinished(self):
        valido=True
        if len(self.entalpia.text())>0:
            try:
                self.entalpia.setText(config.representacion(float(self.entalpia.text().replace(',', '.'))))
            except ValueError:
                self.entalpia.setText("")
                valido=False
        if self.presion.text()<>""and valido:
            self.calcularPropiedades()

    def entropia_editingFinished(self):
        valido=True
        if len(self.entropia.text())>0:
            try:
                self.entropia.setText(config.representacion(float(self.entropia.text().replace(',', '.'))))
            except ValueError:
                self.entropia.setText("")
                valido=False
        if self.variablesCalculo.currentIndex()==2:
            otro=self.presion
        else:
            otro=self.temperatura
        if otro.text()<>"" and valido:
            self.calcularPropiedades()

    def volumen_editingFinished(self):
        valido=True
        if len(self.volumen.text())>0:
            try:
                self.volumen.setText(config.representacion(float(self.volumen.text().replace(',', '.'))))
            except ValueError:
                self.volumen.setText("")
                valido=False
            if self.presion.text()<>"" and valido:
                self.calcularPropiedades()

    def fraccionVapor_editingFinished(self):
        valido=True
        if len(self.fraccionVapor.text())>0:
            try:
                self.fraccionVapor.setText(config.representacion(float(self.fraccionVapor.text().replace(',', '.'))))
            except ValueError:
                self.fraccionVapor.setText("")
                valido=False
            if float(self.fraccionVapor.text())>1.0 or float(self.fraccionVapor.text())<0.0:
                self.fraccionVapor.setText("")
                self.fraccionVapor.setFocus()
                valido=False
            if self.temperatura.text()<>"" and valido:
                self.calcularPropiedades()

    #Dialogos de conversión de unidades
    def unidadesPresion_clicked(self):
        dialog=pressure.Ui_presion()
        dialog.setupUi(dialog)
        try:
            self.p=unidades.Pressure(float(self.presion.text())).unit(config.Configuracion("Pressure").func())
            dialog.Pa.setText(str(self.p))
            dialog.actualizar_Pa()
        except ValueError: pass
        if dialog.exec_():
            self.presion.setText(config.representacion(dialog.dato.config))
            self.calcularPropiedades()

    def unidadesTemperatura_clicked(self):
        dialog=temperature.Ui_temperatura()
        dialog.setupUi(dialog)
        try:
            self.t=unidades.Temperature(float(self.temperatura.text())).unit(config.Configuracion("Temperature").func())
            dialog.kelvin.setText(str(self.t))
            dialog.actualizar_k()
        except ValueError: pass
        if dialog.exec_():
            self.temperatura.setText(config.representacion(dialog.dato.config))
            self.calcularPropiedades()
          
    def unidadesVolumen_clicked(self):
        dialog=specific_volume.Ui_volumen_especifico()
        dialog.setupUi(dialog)
        try:
            self.v=unidades.SpecificVolume(float(self.volumen.text())).unit(config.Configuracion("SpecificVolume").func())
            dialog.m3kg.setText(str(self.v))
            dialog.actualizar_m3kg()
        except ValueError: pass
        if dialog.exec_():
            self.volumen.setText(config.representacion(dialog.dato.config))
            self.calcularPropiedades()
        
    def unidadesEntalpia_clicked(self):
        dialog=enthalpy.Ui_entalpia()
        dialog.setupUi(dialog)
        try:
            self.h=unidades.Enthalpy(float(self.entalpia.text())).unit(config.Configuracion("Enthalpy").func())
            dialog.Jkg.setText(str(self.h))
            dialog.actualizar_Jkg()
        except ValueError: pass
        if dialog.exec_():
            self.entalpia.setText(config.representacion(dialog.dato.config))
            self.calcularPropiedades()
        
    def unidadesEntropia_clicked(self):
        dialog=entropy.Ui_entropia()
        dialog.setupUi(dialog)
        try:
            self.s=unidades.SpecificHeat(float(self.entropia.text())).unit(config.Configuracion("SpecificHeat").func())
            dialog.JkgK.setText(str(self.s))
            dialog.actualizar_JkgK()
        except ValueError: pass
        if dialog.exec_():
            self.entropia.setText(config.representacion(dialog.dato.config))
            self.calcularPropiedades()


    def calcularPropiedades(self):
        """Calcula y muestra las propiedades del punto especificado"""
        if self.variablesCalculo.currentIndex()==0:
            p=unidades.Pressure(float(self.presion.text())).unit(config.Configuracion("Pressure").func())
            T=unidades.Temperature(float(self.temperatura.text())).unit(config.Configuracion("Temperature").func())
            vapor=steam_pT(p, T)
        elif self.variablesCalculo.currentIndex()==1:
            p=unidades.Pressure(float(self.presion.text())).unit(config.Configuracion("Pressure").func())
            h=unidades.Enthalpy(float(self.entalpia.text())).unit(config.Configuracion("Enthalpy").func())
            vapor=steam_ph(p, h)
        elif self.variablesCalculo.currentIndex()==2:
            p=unidades.Pressure(float(self.presion.text())).unit(config.Configuracion("Pressure").func())
            s=unidades.SpecificHeat(float(self.entropia.text())).unit(config.Configuracion("SpecificHeat", "Entropy").func())
            vapor=steam_ps(p, s)            
        elif self.variablesCalculo.currentIndex()==3:
            p=unidades.Pressure(float(self.presion.text())).unit(config.Configuracion("Pressure").func())
            v=unidades.SpecificVolume(self.volumen.text()).unit(config.Configuracion("SpecificVolume").func())
            vapor=steam_pv(p, v)            
        elif self.variablesCalculo.currentIndex()==4:
            T=unidades.Temperature(float(self.temperatura.text())).unit(config.Configuracion("Temperature").func())
            s=unidades.SpecificHeat(float(self.entropia.text())).unit(config.Configuracion("SpecificHeat", "Entropy").func())
            vapor=steam_Ts(T, s)            
        elif self.variablesCalculo.currentIndex()==5:
            T=unidades.Temperature(float(self.temperatura.text())).unit(config.Configuracion("Temperature").func())
            x=float(self.fraccionVapor.text())
            vapor=steam_Tx(T, x)  
        self.punto=vapor
        self.mostrarPropiedades()
        self.botonAdd.setEnabled(True) 
        
    def english(self, bool):
        """Configura las unidades acorde con el sistema británico de unidades"""
        self.semaforo.acquire(1)
        self.conf_temperatura.setCurrentIndex(self.conf_temperatura.findText('F'))
        self.conf_presion.setCurrentIndex(self.conf_presion.findText('psi'))
        self.conf_volumen.setCurrentIndex(self.conf_volumen.findText(u'ft³/lb'))
        self.conf_densidad.setCurrentIndex(self.conf_densidad.findText(u'lb/ft³'))
        self.conf_conductividad.setCurrentIndex(self.conf_conductividad.findText(u'Btu/h·ft·F'))
        self.conf_capacidad.setCurrentIndex(self.conf_capacidad.findText(u'Btu/lb·F'))
        self.conf_entalpia.setCurrentIndex(self.conf_entalpia.findText(u'Btu/lb'))
        self.conf_velocidad.setCurrentIndex(self.conf_velocidad.findText(u'ft/s'))
        self.conf_viscosidad.setCurrentIndex(self.conf_viscosidad.findText(u'cP'))
        self.conf_entropia.setCurrentIndex(self.conf_entropia.findText(u'Btu/lb·F'))
        self.semaforo.release(1)
        self.actualizarConfiguracionUnidades()
        
    def si(self, bool):
        """Configura las unidades acorde con el sistema internacional de unidades"""
        self.semaforo.acquire(1)
        self.conf_temperatura.setCurrentIndex(self.conf_temperatura.findText('K'))
        self.conf_presion.setCurrentIndex(self.conf_presion.findText('bar'))
        self.conf_volumen.setCurrentIndex(self.conf_volumen.findText(u'm³/kg'))
        self.conf_densidad.setCurrentIndex(self.conf_densidad.findText(u'kg/m³'))
        self.conf_conductividad.setCurrentIndex(self.conf_conductividad.findText(u'W/m·K'))
        self.conf_capacidad.setCurrentIndex(self.conf_capacidad.findText(u'kJ/kg·K'))
        self.conf_entalpia.setCurrentIndex(self.conf_entalpia.findText(u'kJ/kg'))
        self.conf_velocidad.setCurrentIndex(self.conf_velocidad.findText(u'm/s'))
        self.conf_viscosidad.setCurrentIndex(self.conf_viscosidad.findText(u'Pa·s'))
        self.conf_entropia.setCurrentIndex(self.conf_entropia.findText(u'kJ/kg·K'))
        self.semaforo.release(1)
        self.actualizarConfiguracionUnidades()
        
    #Opciones gráficos
    def d2(self, activado):
        """Se ejecuta si se cambia el modo de gráfico a 2D"""
        self.diagrama2D.setVisible(activado)
        self.toolbar2D.setVisible(self.actionMostrarBarra.isChecked() and activado)
        self.checkIsoBar.setEnabled(True)
        self.checkIsoTherm.setEnabled(True)
        self.checkIsoEntr.setEnabled(True)
        self.checkIsoEnth.setEnabled(True)
        self.checkIsoVol.setEnabled(True)
        self.diagrama2D.draw()
        
    def d3(self, activado):
        """Se ejecuta si se cambia el modo de gráfico a 3D"""
        self.diagrama3D.setVisible(activado)
        self.toolbar3D.setVisible(self.actionMostrarBarra.isChecked() and activado)
        if activado:
            self.ejesTabla_currentIndexChanged(self.ejesTabla.currentIndex())
            self.diagrama3D.draw()

    def mostrarBarra(self, bool):
        """Muestra la toolbar de matplotlib"""
        self.toolbar2D.setVisible(bool and self.action2D.isChecked())
        self.toolbar3D.setVisible(bool and self.action3D.isChecked())

    def mostrarPuntos(self, bool):
        """Muestra los puntos específicos"""
        for i in self.diagrama3D.puntos:
            i[0].set_visible(bool)
        for i in self.diagrama2D.puntos:
            i[0].set_visible(bool)
        self.diagrama3D.draw()
        self.diagrama2D.draw()
        
    def mostrarSaturacion(self, bool):
        """Muestra la línea de saturación"""
        self.diagrama3D.satliq[0].set_visible(bool)
        self.diagrama3D.satgas[0].set_visible(bool)
        self.diagrama2D.satliq[0].set_visible(bool)
        self.diagrama2D.satgas[0].set_visible(bool)
        self.diagrama3D.draw()
        self.diagrama2D.draw()

    def mostrarIsoentropica(self):
        """Muestra las líneas isoentrópicas"""
        for i in self.diagrama3D.Isoentropica:
            i[0].set_visible(self.checkIsoEntr.isChecked() and self.checkIsoEntr.isEnabled())
        for i in self.diagrama2D.Isoentropica:
            i[0].set_visible(self.checkIsoEntr.isChecked())
        for i in self.diagrama2D.Isoentropica_label:
            i.set_visible(self.checkIsoEntr.isChecked() and self.checkIsoEntrLabel.isChecked())
        self.diagrama3D.draw()
        self.diagrama2D.draw()
        
    def mostrarIsoentalpica(self):
        """Muestra las líneas isoentálpicas"""
        for i in self.diagrama3D.Isoentalpica:
            i[0].set_visible(self.checkIsoEnth.isChecked() and  self.checkIsoEnth.isEnabled())
        for i in self.diagrama2D.Isoentalpica:
            i[0].set_visible(self.checkIsoEnth.isChecked())
        for i in self.diagrama2D.Isoentalpica_label:
            i.set_visible(self.checkIsoEnth.isChecked() and self.checkIsoEnthLabel.isChecked())
        self.diagrama3D.draw()
        self.diagrama2D.draw()

    def mostrarIsobara(self):
        """Muestra las líneas isobáras"""
        for i in self.diagrama3D.Isobara:
            i[0].set_visible(self.checkIsoBar.isChecked() and self.checkIsoBar.isEnabled())
        for i in self.diagrama2D.Isobara:
            i[0].set_visible(self.checkIsoBar.isChecked())
        for i in self.diagrama2D.Isobara_label:
            i.set_visible(self.checkIsoBar.isChecked() and self.checkIsoBarLabel.isChecked())
        self.diagrama3D.draw()
        self.diagrama2D.draw()
        
    def mostrarIsoterma(self):
        """Muestra las líneas isotermas"""
        for i in self.diagrama3D.Isoterma:
            i[0].set_visible(self.checkIsoTherm.isChecked() and self.checkIsoTherm.isEnabled())
        for i in self.diagrama2D.Isoterma:
            i[0].set_visible(self.checkIsoTherm.isChecked())
        for i in self.diagrama2D.Isoterma_label:
            i.set_visible(self.checkIsoTherm.isChecked() and self.checkIsoThermLabel.isChecked())
        self.diagrama3D.draw()
        self.diagrama2D.draw()
        
    def mostrarIsocora(self):
        """Muestra las líneas isocoras"""
        for i in self.diagrama3D.Isocora:
            i[0].set_visible(self.checkIsoVol.isChecked() and self.checkIsoVol.isEnabled())
        for i in self.diagrama2D.Isocora:
            i[0].set_visible(self.checkIsoVol.isChecked())
        for i in self.diagrama2D.Isocora_label:
            i.set_visible(self.checkIsoVol.isChecked() and self.checkIsoVolLabel.isChecked())
        self.diagrama3D.draw()
        self.diagrama2D.draw()
        
    def mostrarIsoX(self):
        """Muestra las líneas con igual fración de vapor"""
        for i in self.diagrama3D.IsoX:
            i[0].set_visible(self.checkIsoX.isChecked())
        for i in self.diagrama2D.IsoX:
            i[0].set_visible(self.checkIsoX.isChecked())
        for i in self.diagrama2D.IsoX_label:
            i.set_visible(self.checkIsoX.isChecked() and self.checkIsoXLabel.isChecked())
        self.diagrama3D.draw()
        self.diagrama2D.draw()

    
    def PersonalizarIsobara_toggled(self, activado):
        self.P_ini.setDisabled(activado)
        self.P_fin.setDisabled(activado)
        self.P_intervalo.setDisabled(activado)
        self.ListaIsobara.setEnabled(activado)

    def PersonalizarIsoterma_toggled(self, activado):
        self.T_ini.setDisabled(activado)
        self.T_fin.setDisabled(activado)
        self.T_intervalo.setDisabled(activado)
        self.ListaIsoterma.setEnabled(activado)

    def PersonalizarIsoentalpica_toggled(self, activado):
        self.H_ini.setDisabled(activado)
        self.H_fin.setDisabled(activado)
        self.H_intervalo.setDisabled(activado)
        self.ListaIsoentalpica.setEnabled(activado)

    def PersonalizarIsoentropica_toggled(self, activado):
        self.S_ini.setDisabled(activado)
        self.S_fin.setDisabled(activado)
        self.S_intervalo.setDisabled(activado)
        self.ListaIsoentropica.setEnabled(activado)

    def PersonalizarIsocora_toggled(self, activado):
        self.V_ini.setDisabled(activado)
        self.V_fin.setDisabled(activado)
        self.V_intervalo.setDisabled(activado)
        self.ListaIsocora.setEnabled(activado)
        
    def PersonalizarIsoX_toggled(self, activado):
        self.X_ini.setDisabled(activado)
        self.X_fin.setDisabled(activado)
        self.X_intervalo.setDisabled(activado)
        self.ListaIsoX.setEnabled(activado)
        
    def IsotermaCritica_togggled(self):
        self.calcularIsoterma()
        self.dibujar()
        
    def IsobaraCritica_togggled(self):
        self.calcularIsobara()
        self.dibujar()

    def IsoentalpicaCritica_togggled(self):
        self.calcularIsoentalpica()
        self.dibujar()

    def IsoentropicaCritica_togggled(self):
        self.calcularIsoentropica()
        self.dibujar()

    def IsocoraCritica_togggled(self):
        self.calcularIsocora()
        self.dibujar()

    def IsoXCritica_togggled(self):
        self.calcularIsoX()
        self.dibujar()


    def IsotermaColor_clicked(self):
        """Dialogo de selección de color para las isotermas"""
        dialog=QtGui.QColorDialog(self.IsotermaColor, SteamTables)
        if dialog.exec_():
            self.IsotermaColorButton.setPalette(QtGui.QPalette(dialog.currentColor()))
            self.IsotermaColor=dialog.currentColor()
            self.dibujar()

    def IsobaraColor_clicked(self):
        """Dialogo de selección de color para las isobaras"""
        dialog=QtGui.QColorDialog(self.IsobaraColor, SteamTables)
        if dialog.exec_():
            self.IsobaraColorButton.setPalette(QtGui.QPalette(dialog.currentColor()))
            self.IsobaraColor=dialog.currentColor()
            self.dibujar()

    def IsocoraColor_clicked(self):
        """Dialogo de selección de color para las isocoras"""
        dialog=QtGui.QColorDialog(self.IsocoraColor, SteamTables)
        if dialog.exec_():
            self.IsocoraColorButton.setPalette(QtGui.QPalette(dialog.currentColor()))
            self.IsocoraColor=dialog.currentColor()
            self.dibujar()
            
    def IsoXColor_clicked(self):
        """Dialogo de selección de color para las líneas de igual fracción de vapor"""
        dialog=QtGui.QColorDialog(self.IsoXColor, SteamTables)
        if dialog.exec_():
            self.IsoXColorButton.setPalette(QtGui.QPalette(dialog.currentColor()))
            self.IsoXColor=dialog.currentColor()
            self.dibujar()
            
    def IsoentalpicaColor_clicked(self):
        """Dialogo de selección de color para las isoentálpicas"""
        dialog=QtGui.QColorDialog(self.IsoentalpicaColor, SteamTables)
        if dialog.exec_():
            self.IsoentalpicaColorButton.setPalette(QtGui.QPalette(dialog.currentColor()))
            self.IsoentalpicaColor=dialog.currentColor()
            self.dibujar()

    def IsoentropicaColor_clicked(self):
        """Dialogo de selección de color para las isoentrópicas"""
        dialog=QtGui.QColorDialog(self.IsoentropicaColor, SteamTables)
        if dialog.exec_():
            self.IsoentropicaColorButton.setPalette(QtGui.QPalette(dialog.currentColor()))
            self.IsoentropicaColor=dialog.currentColor()
            self.dibujar()


    def rellenar_variableTabla(self, i, j):
        """Actualiza los elementos disponibles para el tercer eje en función de los elejidos como ejes principales i, j"""
        self.variableTabla.clear()
        variables=[QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8),  QtGui.QApplication.translate("SteamTables", "Volumen específico", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Entalpía", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Entropía", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Energía interna", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Energía de Gibbs", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Energía de Helmholtz", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Cp", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Cv", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Densidad", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Conductividad térmica", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Viscosidad", None, QtGui.QApplication.UnicodeUTF8),  QtGui.QApplication.translate("SteamTables", "Fracción de vapor", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("SteamTables", "Velocidad del sonido", None, QtGui.QApplication.UnicodeUTF8)]
        del variables[j]
        del variables[i]
        for nombre in variables:
            self.variableTabla.addItem(nombre)

    def rellenar_ejeY(self, int):
        """Rellena las variables disponibles para el ejeY en el gráfico 2D, todos menos el que este activo en el ejeX"""
        self.ejeY.clear()
        Ejes2D=["p", "T", "s", "h", "u", "v"]
        del Ejes2D[int]
        for i in Ejes2D:
            self.ejeY.addItem(i)

    def ejesTabla_currentIndexChanged(self, indice):
        """Configura diferentes cosas necesarias cuando se cambian los ejes de la tabla"""
        if indice==0:
            self.rellenar_variableTabla(0, 1)
            self.label_26.setText(QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Pressure").text())
            self.label_27.setText(QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Temperature").text())
            self.checkIsoBar.setEnabled(False)
            self.checkIsoTherm.setEnabled(False)
            self.checkIsoEntr.setEnabled(True)
            self.checkIsoEnth.setEnabled(True)
            self.checkIsoVol.setEnabled(True)
        elif indice==1:
            self.rellenar_variableTabla(0, 3)
            self.label_26.setText(QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Pressure").text())
            self.label_27.setText(QtGui.QApplication.translate("SteamTables", "Entalpía", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Enthalpy").text())
            self.checkIsoBar.setEnabled(False)
            self.checkIsoTherm.setEnabled(True)
            self.checkIsoEntr.setEnabled(True)
            self.checkIsoEnth.setEnabled(False)
            self.checkIsoVol.setEnabled(True)
        elif indice==2:
            self.rellenar_variableTabla(0, 4)
            self.label_26.setText(QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Pressure").text())
            self.label_27.setText(QtGui.QApplication.translate("SteamTables", "Entropía", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("SpecificHeat", "Entropy").text())
            self.checkIsoBar.setEnabled(False)
            self.checkIsoTherm.setEnabled(True)
            self.checkIsoEntr.setEnabled(False)
            self.checkIsoEnth.setEnabled(True)
            self.checkIsoVol.setEnabled(True)
        elif indice==3:
            self.rellenar_variableTabla(0, 2)
            self.label_26.setText(QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Pressure").text())
            self.label_27.setText(QtGui.QApplication.translate("SteamTables", "Volumen", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("SpecificVolume").text())
            self.checkIsoBar.setEnabled(False)
            self.checkIsoTherm.setEnabled(True)
            self.checkIsoEntr.setEnabled(True)
            self.checkIsoEnth.setEnabled(True)
            self.checkIsoVol.setEnabled(False)
        elif indice==4:
            self.rellenar_variableTabla(1, 4)
            self.label_26.setText(QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Temperature").text())
            self.label_27.setText(QtGui.QApplication.translate("SteamTables", "Entropía", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("SpecificHeat", "Entropy").text())
            self.checkIsoBar.setEnabled(True)
            self.checkIsoTherm.setEnabled(False)
            self.checkIsoEntr.setEnabled(False)
            self.checkIsoEnth.setEnabled(True)
            self.checkIsoVol.setEnabled(True)
        elif indice==5:
            self.rellenar_variableTabla(1, 13)
            self.label_26.setText(QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8)+", %s" %config.Configuracion("Temperature").text())
            self.label_27.setText(QtGui.QApplication.translate("SteamTables", "Calidad", None, QtGui.QApplication.UnicodeUTF8))
            self.checkIsoBar.setEnabled(True)
            self.checkIsoTherm.setEnabled(False)
            self.checkIsoEntr.setEnabled(True)
            self.checkIsoEnth.setEnabled(True)
            self.checkIsoVol.setEnabled(True)
    
    
    def botonCalcular_clicked(self):
        """Método que calcula todos los datos de las tablas y gráficos"""
        self.progresbar.setVisible(True)
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Calculando matriz...", None, QtGui.QApplication.UnicodeUTF8))
        self.progresbar.setValue(0)
        QtGui.QApplication.processEvents()
        self.calcularMatriz(0, 30)
        self.Calcular_Propiedades(35, 5)
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Calculando línea de saturación...", None, QtGui.QApplication.UnicodeUTF8))
        QtGui.QApplication.processEvents()
        self.calcularSaturacion()
        if len(self.puntos)>0:
            self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Calculando puntos personalizados...", None, QtGui.QApplication.UnicodeUTF8))
            QtGui.QApplication.processEvents()
            self.calcularPuntos()
        self.calcularIsoterma(40, 10)
        self.calcularIsobara(50, 10)
        self.calcularIsoentropica(60, 10)
        self.calcularIsoentalpica(70, 10)
        self.calcularIsocora(80, 10)
        self.calcularIsoX(90, 10)
        self.progresbar.setValue(100)
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Dibujando...", None, QtGui.QApplication.UnicodeUTF8))
        QtGui.QApplication.processEvents()
        self.dibujar()
        self.progresbar.setVisible(False)
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Listo", None, QtGui.QApplication.UnicodeUTF8))
        
    def factores_conversion(self):
        """Método que calcula los factores de conversión de unidades necesarios, tambien los textos"""
        if self.conf_temperatura.currentText()=="K":
            self.conv_T=float
            self.conv_T_inv=float
        elif self.conf_temperatura.currentText()=="C":
            self.conv_T=config.C2K
            self.conv_T_inv=config.K2C
        elif self.conf_temperatura.currentText()=="F":
            self.conv_T=config.F2K
            self.conv_T_inv=config.K2F
        elif self.conf_temperatura.currentText()=="R":
            self.conv_T=config.R2K
            self.conv_T_inv=config.K2R
        elif self.conf_temperatura.currentText()=="Re":
            self.conv_T=config.Re2K
            self.conv_T_inv=config.K2Re
            
        if self.ejesTabla.currentIndex()==0:
            abcisa="P, %s" % config.Configuracion("Pressure").text()
            ordenada="T, %s" % config.Configuracion("Temperature").text()
            self.factorx=unidades.Pressure(1).unit(config.Configuracion("Pressure").func())
            self.factory=0
        elif self.ejesTabla.currentIndex()==1:
            abcisa="P, %s" % config.Configuracion("Pressure").text()
            ordenada="H, %s" % config.Configuracion("Enthalpy").text()
            self.factorx=unidades.Pressure(1).unit(config.Configuracion("Pressure").func())
            self.factory=unidades.Enthalpy(1).unit(config.Configuracion("Enthalpy").func())
        elif self.ejesTabla.currentIndex()==2:
            abcisa="P, %s" % config.Configuracion("Pressure").text()
            ordenada="S, %s" % config.Configuracion("SpecificHeat","Entropy").text()
            self.factorx=unidades.Pressure(1).unit(config.Configuracion("Pressure").func())
            self.factory=unidades.SpecificHeat(1).unit(config.Configuracion("SpecificHeat","Entropy").func())
        elif self.ejesTabla.currentIndex()==3:
            abcisa="P, %s" % config.Configuracion("Pressure").text()
            ordenada="v, %s" % config.Configuracion("SpecificVolume").text()
            self.factorx=unidades.Pressure(1).unit(config.Configuracion("Pressure").func())
            self.factory=unidades.SpecificVolume(1).unit(config.Configuracion("SpecificVolume").func())
        elif self.ejesTabla.currentIndex()==4:
            abcisa="T, %s" % config.Configuracion("Temperature").text()
            ordenada="S, %s" % config.Configuracion("SpecificHeat","Entropy").text()
            self.factorx=0
            self.factory=unidades.SpecificHeat(1).unit(config.Configuracion("SpecificHeat","Entropy").func())
        elif self.ejesTabla.currentIndex()==5:
            abcisa="T, %s" % config.Configuracion("Temperature").text()
            ordenada="x"
            self.factorx=0
            self.factory=1
            
        if self.ejeX.currentText()=="p":
            abcisa2="P, %s" % config.Configuracion("Pressure").text()
            self.factorx2=unidades.Pressure(1).unit(config.Configuracion("Pressure").func())
        elif self.ejeX.currentText()=="T":
            abcisa2="T, %s" % config.Configuracion("Temperature").text()
            self.factorx2=0
        elif self.ejeX.currentText()=="h":
            abcisa2="H, %s" % config.Configuracion("Enthalpy").text()
            self.factorx2=unidades.Enthalpy(1).unit(config.Configuracion("Enthalpy").func())
        elif self.ejeX.currentText()=="v":
            abcisa2="v, %s" % config.Configuracion("SpecificVolume").text()
            self.factorx2=unidades.SpecificVolume(1).unit(config.Configuracion("SpecificVolume").func())
        elif self.ejeX.currentText()=="s":
            abcisa2="S, %s" % config.Configuracion("SpecificHeat","Entropy").text()
            self.factorx2=unidades.SpecificHeat(1).unit(config.Configuracion("SpecificHeat","Entropy").func())
        elif self.ejeX.currentText()=="u":
            abcisa2="U, %s" %config.Configuracion("Enthalpy").text()
            self.factorx2=unidades.Enthalpy(1).unit(config.Configuracion("Enthalpy").func())
        if self.ejeY.currentText()=="p":
            ordenada2="P, %s" % config.Configuracion("Pressure").text()
            self.factory2=unidades.Pressure(1).unit(config.Configuracion("Pressure").func())
        elif self.ejeY.currentText()=="T":
            ordenada2="T, %s" % config.Configuracion("Temperature").text()
            self.factory2=0
        elif self.ejeY.currentText()=="h":
            ordenada2="H, %s" % config.Configuracion("Enthalpy").text()
            self.factory2=unidades.Enthalpy(1).unit(config.Configuracion("Enthalpy").func())
        elif self.ejeY.currentText()=="v":
            ordenada2="v, %s" % config.Configuracion("SpecificVolume").text()
            self.factory2=unidades.SpecificVolume(1).unit(config.Configuracion("SpecificVolume").func())
        elif self.ejeY.currentText()=="s":
            ordenada2="S, %s" % config.Configuracion("SpecificHeat","Entropy").text()
            self.factory2=unidades.SpecificHeat(1).unit(config.Configuracion("SpecificHeat","Entropy").func())
        elif self.ejeY.currentText()=="u":
            ordenada2="U, %s" %config.Configuracion("Enthalpy").text()
            self.factory2=unidades.Enthalpy(1).unit(config.Configuracion("Enthalpy").func())

        if self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8):
            texto="P, %s" %config.Configuracion("Pressure").text()
            self.factorz=unidades.Pressure(1).unit(config.Configuracion("Pressure").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8):
            texto="T, %s" %config.Configuracion("Temperature").text()
            self.factorz=0
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Volumen específico", None, QtGui.QApplication.UnicodeUTF8):
            texto="v, %s" %config.Configuracion("SpecificVolume").text()
            self.factorz=unidades.SpecificVolume(1).unit(config.Configuracion("SpecificVolume").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Entalpía", None, QtGui.QApplication.UnicodeUTF8):
            texto="H, %s" %config.Configuracion("Enthalpy").text()
            self.factorz=unidades.Enthalpy(1).unit(config.Configuracion("Enthalpy").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Entropía", None, QtGui.QApplication.UnicodeUTF8):
            texto="S, %s" %config.Configuracion("SpecificHeat", "Entropy").text()
            self.factorz=unidades.SpecificHeat(1).unit(config.Configuracion("SpecificHeat", "Entropy").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía interna", None, QtGui.QApplication.UnicodeUTF8):
            texto="U, %s" %config.Configuracion("Enthalpy").text()
            self.factorz=unidades.Enthalpy(1).unit(config.Configuracion("Enthalpy").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Cp", None, QtGui.QApplication.UnicodeUTF8):
            texto="Cp, %s" %config.Configuracion("SpecificHeat").text()
            self.factorz=unidades.SpecificHeat(1).unit(config.Configuracion("SpecificHeat").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Cv", None, QtGui.QApplication.UnicodeUTF8):
            texto="Cv, %s" %config.Configuracion("SpecificHeat").text()
            self.factorz=unidades.SpecificHeat(1).unit(config.Configuracion("SpecificHeat").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Densidad", None, QtGui.QApplication.UnicodeUTF8):
            texto=u"ρ, %s" %config.Configuracion("Density").text()
            self.factorz=unidades.Density(1).unit(config.Configuracion("Density").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Conductividad térmica", None, QtGui.QApplication.UnicodeUTF8):
            texto="k, %s" %config.Configuracion("ThermalConductivity").text()
            self.factorz=unidades.ThermalConductivity(1).unit(config.Configuracion("ThermalConductivity").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Viscosidad", None, QtGui.QApplication.UnicodeUTF8):
            texto=u"μ, %s" %config.Configuracion("Viscosity").text()
            self.factorz=unidades.Viscosity(1).unit(config.Configuracion("Viscosity").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Velocidad del sonido", None, QtGui.QApplication.UnicodeUTF8):
            texto="w, %s" %config.Configuracion("Speed").text()
            self.factorz=unidades.Speed(1).unit(config.Configuracion("Speed").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Fracción de vapor", None, QtGui.QApplication.UnicodeUTF8):
            texto="x"
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía de Gibbs", None, QtGui.QApplication.UnicodeUTF8):
            texto="G, %s" %config.Configuracion("Enthalpy").text()
            self.factorz=unidades.Enthalpy(1).unit(config.Configuracion("Enthalpy").func())
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía de Helmholtz", None, QtGui.QApplication.UnicodeUTF8):
            texto="A, %s" %config.Configuracion("Enthalpy").text()
            self.factorz=unidades.Enthalpy(1).unit(config.Configuracion("Enthalpy").func())
            
        self.etiquetas=[abcisa, ordenada, texto]
        self.etiquetas2=[abcisa2, ordenada2]


    def calcularMatriz(self, start=0, rango=40):
        """Método que actualiza los datos de matriz"""        
        xini=float(self.abscisaInicio.text())
        xfin=float(self.abscisaFin.text())
        xsalto=float(self.abscisaIntervalo.text())                
        xn=int((xfin-xini)/xsalto+1)
        yini=float(self.ordenadaInicio.text())
        yfin=float(self.ordenadaFin.text())
        ysalto=float(self.ordenadaIntervalo.text())
        yn=int((yfin-yini)/ysalto+1)
        self.tabla.setRowCount(yn)
        self.tabla.setColumnCount(xn)
        
        self.factores_conversion()
                    
        xi=arange(xini, xfin, xsalto)
        if (xfin-xini)/xsalto==float(int((xfin-xini)/xsalto)):
            xi=concatenate((xi, [xfin]))
        yi=arange(yini, yfin, ysalto)
        if (yfin-yini)/ysalto==float(int((yfin-yini)/ysalto)):
            yi=concatenate((yi, [yfin]))

        for i in range(len(xi)):
            headerItem = QtGui.QTableWidgetItem()
            headerItem.setText(str(xi[i]))
            self.tabla.setHorizontalHeaderItem(i,headerItem)
        for i in range(len(yi)):
            headerItem = QtGui.QTableWidgetItem()
            headerItem.setText(str(yi[i]))
            self.tabla.setVerticalHeaderItem(i,headerItem)
            
        xdata,ydata = meshgrid(xi, yi)
        self.matriz=[]

        for i in range(len(xi)):
            self.progresbar.setValue(start+rango*(i+1.)/len(xi))
            QtGui.QApplication.processEvents()
            fila=[]
            for j in range(len(yi)):
                if self.ejesTabla.currentIndex()==0:
                    vapor=steam_pT(xi[i]*self.factorx, self.conv_T(yi[j]))
                elif self.ejesTabla.currentIndex()==1:
                    vapor=steam_ph(xi[i]*self.factorx, yi[j]*self.factory)
                elif self.ejesTabla.currentIndex()==2:
                    vapor=steam_ps(xi[i]*self.factorx, yi[j]*self.factory)       
                elif self.ejesTabla.currentIndex()==3:
                    vapor=steam_pv(xi[i]*self.factorx, yi[j]*self.factory)     
                elif self.ejesTabla.currentIndex()==4:
                    if bounds_Ts(xi[i], yi[j], 0)==0:
                        vapor=steam_Ts(self.conv_T(xi[i]), yi[j]*self.factory)
                    else:
                        vapor=steam_Ts(TCRIT, steam_pT(PCRIT, TCRIT).s)
                elif self.ejesTabla.currentIndex()==5:
                    vapor=steam_Tx(self.conv_T(xi[i]), yi[j]) 
                fila.append(vapor)
                    
            self.matriz.append(fila)

        self.xdata=xdata
        self.ydata=ydata
        self.actionCSV.setEnabled(True)


    def Calcular_Propiedades(self, start, rango):
        """Método que actualiza los datos al cambiar la propiedad a mostrar"""
        if len(self.matriz)!=0:
            zdata = zeros(self.xdata.shape)
            
            for i, fila in enumerate(self.matriz):
                self.progresbar.setValue(start+rango*(i+1.)/len(self.matriz))
                QtGui.QApplication.processEvents()
                for j, vapor in enumerate(fila):
                    if self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.p/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8):
                        dato=self.conv_T_inv(vapor.T)
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Volumen específico", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.v/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Entalpía", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.h/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Entropía", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.s/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía interna", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.u/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Cp", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.cp/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Cv", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.cv/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Densidad", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.rho/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Conductividad térmica", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.k/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Viscosidad", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.mu/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Velocidad del sonido", None, QtGui.QApplication.UnicodeUTF8):
                        if vapor.region !='\x04' and vapor.region !='\x03':
                            dato=vapor.w/self.factorz
                        else:
                            dato=0.0
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Fracción de vapor", None, QtGui.QApplication.UnicodeUTF8):
                        dato=vapor.x
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía de Gibbs", None, QtGui.QApplication.UnicodeUTF8):
                        dato=(vapor.h-vapor.T*vapor.s)/self.factorz
                    elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía de Helmholtz", None, QtGui.QApplication.UnicodeUTF8):
                        dato=(vapor.u-vapor.T*vapor.s)/self.factorz
                    zdata[j, i]=dato

            for i, fila in enumerate(zdata):
                for j, dato in enumerate(fila):
                    self.tabla.setItem(i, j,QtGui.QTableWidgetItem(config.representacion(dato)))
                    self.tabla.item(i, j).setTextAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)

            self.tabla.resizeColumnsToContents()
            self.toolBox.setItemText(self.toolBox.indexOf(self.page_1), QtGui.QApplication.translate("SteamTables", "Tabla", None, QtGui.QApplication.UnicodeUTF8)+"   %s - %s - %s" %(self.etiquetas[0], self.etiquetas[1], self.etiquetas[2]))
            self.zdata=zdata


    def calcularSaturacion(self):
        """Método que calcula datos de la línea de saturación"""
#TODO: Es mejor opción dibujar solo las puntos de la linea de saturación en los ejes, de momento se dibujara toda la linea aunque se salga de los ejes del gráfico
        TT0 = linspace(273.15, TCRIT, 100)
        psat = [psat_T(T) for T in TT0]
        
        if self.ejesTabla.currentIndex()==0:
            self.xsat=[[P/self.factorx for P in psat], [P/self.factorx for P in psat]]
            self.ysat=[[self.conv_T_inv(T) for T in TT0], [self.conv_T_inv(T) for T in TT0]]
        elif self.ejesTabla.currentIndex()==1:
            self.xsat=[[P/self.factorx for P in psat], [P/self.factorx for P in psat]]
            self.ysat=[[region4_Tx(T,0).h/self.factory for T in TT0], [region4_Tx(T,1).h/self.factory for T in TT0]]
        elif self.ejesTabla.currentIndex()==2:
            self.xsat=[[P/self.factorx for P in psat], [P/self.factorx for P in psat]]
            self.ysat=[[region4_Tx(T,0).s/self.factory for T in TT0], [region4_Tx(T,1).s/self.factory for T in TT0]]
        elif self.ejesTabla.currentIndex()==3:
            self.xsat=[[P/self.factorx for P in psat], [P/self.factorx for P in psat]]
            self.ysat=[[region4_Tx(T,0).v/self.factory for T in TT0], [region4_Tx(T,1).v/self.factory for T in TT0]]
        elif self.ejesTabla.currentIndex()==4:
            self.xsat=[[self.conv_T_inv(T) for T in TT0], [self.conv_T_inv(T) for T in TT0]]
            self.ysat=[[region4_Tx(T,0).s/self.factory for T in TT0], [region4_Tx(T,1).s/self.factory for T in TT0]]
        elif self.ejesTabla.currentIndex()==5:
            self.xsat=[[self.conv_T_inv(T) for T in TT0], [self.conv_T_inv(T) for T in TT0]]
            self.ysat=[[0]*100, [1]*100]
        
        if self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[P/self.factorz for P in psat], [P/self.factorz for P in psat]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[self.conv_T_inv(T) for T in TT0], [self.conv_T_inv(T) for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Volumen específico", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[region4_Tx(T,0).v/self.factorz for T in TT0], [region4_Tx(T,1).v/self.factorz for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Entalpía", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[region4_Tx(T,0).h/self.factorz for T in TT0], [region4_Tx(T,1).h/self.factorz for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Entropía", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[region4_Tx(T,0).s/self.factorz for T in TT0], [region4_Tx(T,1).s/self.factorz for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía interna", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[region4_Tx(T,0).u/self.factorz for T in TT0], [region4_Tx(T,1).u/self.factorz for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Cp", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[region4_Tx(T,0).cp/self.factorz for T in TT0], [region4_Tx(T,1).cp/self.factorz for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Cv", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[region4_Tx(T,0).cv/self.factorz for T in TT0], [region4_Tx(T,1).cv/self.factorz for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Densidad", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[region4_Tx(T,0).rho/self.factorz for T in TT0], [region4_Tx(T,1).rho/self.factorz for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Conductividad térmica", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[region4_Tx(T,0).k/self.factorz for T in TT0], [region4_Tx(T,1).k/self.factorz for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Viscosidad", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[region4_Tx(T,0).mu/self.factorz for T in TT0], [region4_Tx(T,1).mu/self.factorz for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Velocidad del sonido", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[0 for T in TT0], [0 for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Fracción de vapor", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[0]*len(TT0), [1]*len(TT0)]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía de Gibbs", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[(region4_Tx(T,0).h-T*region4_Tx(T,0).s)/self.factorz for T in TT0], [(region4_Tx(T,1).h-T*region4_Tx(T,1).s)/self.factorz for T in TT0]]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía de Helmholtz", None, QtGui.QApplication.UnicodeUTF8):
            self.zsat=[[(region4_Tx(T,0).u-T*region4_Tx(T,0).s)/self.factorz for T in TT0], [(region4_Tx(T,1).u-T*region4_Tx(T,1).s)/self.factorz for T in TT0]]
    
        if self.ejeX.currentText()=="p":
            self.xsat2=[[P/self.factorx2 for P in psat], [P/self.factorx2 for P in psat]]
        elif self.ejeX.currentText()=="T":
            self.xsat2=[[self.conv_T_inv(T) for T in TT0], [self.conv_T_inv(T) for T in TT0]]
        elif self.ejeX.currentText()=="h":
            self.xsat2=[[region4_Tx(T,0).h/self.factorx2 for T in TT0], [region4_Tx(T,1).h/self.factorx2 for T in TT0]]
        elif self.ejeX.currentText()=="v":
            self.xsat2=[[region4_Tx(T,0).v/self.factorx2 for T in TT0], [region4_Tx(T,1).v/self.factorx2 for T in TT0]]
        elif self.ejeX.currentText()=="s":
            self.xsat2=[[region4_Tx(T,0).s/self.factorx2 for T in TT0], [region4_Tx(T,1).s/self.factorx2 for T in TT0]]
        elif self.ejeX.currentText()=="u":
            self.xsat2=[[region4_Tx(T,0).u/self.factorx2 for T in TT0], [region4_Tx(T,1).u/self.factorx2 for T in TT0]]
        if self.ejeY.currentText()=="p":
            self.ysat2=[[P/self.factory2 for P in psat], [P/self.factory2 for P in psat]]
        elif self.ejeY.currentText()=="T":
            self.ysat2=[[self.conv_T_inv(T) for T in TT0], [self.conv_T_inv(T) for T in TT0]]
        elif self.ejeY.currentText()=="h":
            self.ysat2=[[region4_Tx(T,0).h/self.factory2 for T in TT0], [region4_Tx(T,1).h/self.factory2 for T in TT0]]
        elif self.ejeY.currentText()=="v":
            self.ysat2=[[region4_Tx(T,0).v/self.factory2 for T in TT0], [region4_Tx(T,1).v/self.factory2 for T in TT0]]
        elif self.ejeY.currentText()=="s":
            self.ysat2=[[region4_Tx(T,0).s/self.factory2 for T in TT0], [region4_Tx(T,1).s/self.factory2 for T in TT0]]
        elif self.ejeY.currentText()=="u":
            self.ysat2=[[region4_Tx(T,0).u/self.factory2 for T in TT0], [region4_Tx(T,1).u/self.factory2 for T in TT0]]


    def calcularPuntos(self):
        """Método que actualiza los datos de puntos definidos por el usuario"""
        if self.ejesTabla.currentIndex()==0:
            self.xi=[punto.p/self.factorx for punto in self.puntos]
            self.yi=[self.conv_T_inv(punto.T) for punto in self.puntos]
        elif self.ejesTabla.currentIndex()==1:
            self.xi=[punto.p/self.factorx for punto in self.puntos]
            self.yi=[punto.h/self.factory for punto in self.puntos]
        elif self.ejesTabla.currentIndex()==2:
            self.xi=[punto.p/self.factorx for punto in self.puntos]
            self.yi=[punto.s/self.factory for punto in self.puntos]
        elif self.ejesTabla.currentIndex()==3:
            self.xi=[punto.p/self.factorx for punto in self.puntos]
            self.yi=[punto.v/self.factory for punto in self.puntos]
        elif self.ejesTabla.currentIndex()==4:
            self.xi=[self.conv_T_inv(punto.T) for punto in self.puntos]
            self.yi=[punto.s/self.factory for punto in self.puntos]
        elif self.ejesTabla.currentIndex()==5:
            self.xi=[self.conv_T_inv(punto.T) for punto in self.puntos]
            self.yi=[punto.x for punto in self.puntos]
            
        if self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.p/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[self.conv_T_inv(punto.T) for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Volumen específico", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.v/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Entalpía", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.h/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Entropía", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.s/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía interna", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.u/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Cp", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.cp/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Cv", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.cv/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Densidad", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.rho/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Conductividad térmica", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.k/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Viscosidad", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.mu/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Velocidad del sonido", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[0 for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Fracción de vapor", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[punto.x for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía de Gibbs", None, QtGui.QApplication.UnicodeUTF8):
            self.zi=[(punto.h-punto.T*punto.s)/self.factorz for punto in self.puntos]
        elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía de Helmholtz", None, QtGui.QApplication.UnicodeUTF8):   
            self.zi=[(punto.u-punto.T*punto.s)/self.factorz for punto in self.puntos]
                
        if self.ejeX.currentText()=="p":
            self.xi2=[punto.p/self.factorx2 for punto in self.puntos]
        elif self.ejeX.currentText()=="T":
            self.xi2=[self.conv_T_inv(punto.T) for punto in self.puntos]
        elif self.ejeX.currentText()=="h":
            self.xi2=[punto.h/self.factorx2 for punto in self.puntos]
        elif self.ejeX.currentText()=="v":
            self.xi2=[punto.v/self.factorx2 for punto in self.puntos]
        elif self.ejeX.currentText()=="s":
            self.xi2=[punto.s/self.factorx2 for punto in self.puntos]
        elif self.ejeX.currentText()=="u":
            self.xi2=[punto.u/self.factorx2 for punto in self.puntos]
        if self.ejeY.currentText()=="p":
            self.yi2=[punto.p/self.factory2 for punto in self.puntos]
        elif self.ejeY.currentText()=="T":
            self.yi2=[self.conv_T_inv(punto.T) for punto in self.puntos]
        elif self.ejeY.currentText()=="h":
            self.yi2=[punto.h/self.factory2 for punto in self.puntos]
        elif self.ejeY.currentText()=="v":
            self.yi2=[punto.v/self.factory2 for punto in self.puntos]
        elif self.ejeY.currentText()=="s":
            self.yi2=[punto.s/self.factory2 for punto in self.puntos]
        elif self.ejeY.currentText()=="u":
            self.yi2=[punto.u/self.factory2 for punto in self.puntos]

        
    def isolineas(self, S, X, funcion, start, rango):
        """Librería de cálculo de los parámetros de las isolineas"""
        x=[]
        y=[]
        z=[]
        x2=[]
        y2=[]
        for i, propiedad in enumerate(S):
            self.progresbar.setValue(start+rango*(i+1.)/len(S))
            QtGui.QApplication.processEvents()
            if self.ejesTabla.currentIndex()==0:
                xi=[funcion(i, propiedad).p/self.factorx for i in X]
                yi=[self.conv_T_inv(funcion(i, propiedad).T) for i in X]
            elif self.ejesTabla.currentIndex()==1:
                xi=[funcion(i, propiedad).p/self.factorx for i in X]
                yi=[funcion(i, propiedad).h/self.factory for i in X]
            elif self.ejesTabla.currentIndex()==2:
                xi=[funcion(i, propiedad).p/self.factorx for i in X]
                yi=[funcion(i, propiedad).s/self.factory for i in X]
            elif self.ejesTabla.currentIndex()==3:
                xi=[funcion(i, propiedad).p/self.factorx for i in X]
                yi=[funcion(i, propiedad).v/self.factory for i in X]
            elif self.ejesTabla.currentIndex()==4:
                xi=[self.conv_T_inv(funcion(i, propiedad).T) for i in X]
                yi=[funcion(i, propiedad).s/self.factory for i in X]
            elif self.ejesTabla.currentIndex()==5:
                xi=[self.conv_T_inv(funcion(i, propiedad).T) for i in X]
                yi=[funcion(i, propiedad).x for i in X]
                
            if self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Presión", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).p/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Temperatura", None, QtGui.QApplication.UnicodeUTF8):
                zi=[self.conv_T_inv(funcion(i, propiedad).T) for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Volumen específico", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).v/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Entalpía", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).h/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Entropía", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).s/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía interna", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).u/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Cp", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).cp/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Cv", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).cv/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Densidad", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).rho/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Conductividad térmica", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).k/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Viscosidad", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).mu/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Velocidad del sonido", None, QtGui.QApplication.UnicodeUTF8):
                zi=[0 for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Fracción de vapor", None, QtGui.QApplication.UnicodeUTF8):
                zi=[funcion(i, propiedad).x for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía de Gibbs", None, QtGui.QApplication.UnicodeUTF8):
                zi=[(funcion(i, propiedad).h-funcion(i, propiedad).T*funcion(i, propiedad).s)/self.factorz for i in X]
            elif self.variableTabla.currentText()==QtGui.QApplication.translate("SteamTables", "Energía de Helmholtz", None, QtGui.QApplication.UnicodeUTF8):
                zi=[(funcion(i, propiedad).u-funcion(i, propiedad).T*funcion(i, propiedad).s)/self.factorz for i in X]
                
            if self.ejeX.currentText()=="p":
                xi2=[funcion(i, propiedad).p/self.factorx2 for i in X]
            elif self.ejeX.currentText()=="T":
                xi2=[self.conv_T_inv(funcion(i, propiedad).T) for i in X]
            elif self.ejeX.currentText()=="h":
                xi2=[funcion(i, propiedad).h/self.factorx2 for i in X]
            elif self.ejeX.currentText()=="v":
                xi2=[funcion(i, propiedad).v/self.factorx2 for i in X]
            elif self.ejeX.currentText()=="s":
                xi2=[funcion(i, propiedad).s/self.factorx2 for i in X]
            elif self.ejeX.currentText()=="u":
                xi2=[funcion(i, propiedad).u/self.factorx2 for i in X]
            if self.ejeY.currentText()=="p":
                yi2=[funcion(i, propiedad).p/self.factory2 for i in X]
            elif self.ejeY.currentText()=="T":
                yi2=[self.conv_T_inv(funcion(i, propiedad).T) for i in X]
            elif self.ejeY.currentText()=="h":
                yi2=[funcion(i, propiedad).h/self.factory2 for i in X]
            elif self.ejeY.currentText()=="v":
                yi2=[funcion(i, propiedad).v/self.factory2 for i in X]
            elif self.ejeY.currentText()=="s":
                yi2=[funcion(i, propiedad).s/self.factory2 for i in X]
            elif self.ejeY.currentText()=="u":
                yi2=[funcion(i, propiedad).u/self.factory2 for i in X]
            x.append(xi)
            y.append(yi)
            z.append(zi)
            x2.append(xi2)
            y2.append(yi2)
        return x, y, z, x2, y2
 
    def labels(self, isolineas, x, y, pos):
        x_label=[]
        y_label=[]
        label=[]
        angle=[]
        pos=int(pos/100.*len(x[0]))
        for i in range(len(isolineas)):
            if x[i][pos]<float(self.ejeX_min.text()):
                x_label.append(float(self.ejeX_min.text()))
            elif x[i][pos]>float(self.ejeX_max.text()):
                x_label.append(float(self.ejeX_max.text()))
            else:
                x_label.append(x[i][pos])
            if y[i][pos]<float(self.ejeY_min.text()):
                y_label.append(float(self.ejeY_min.text()))
            elif y[i][pos]>float(self.ejeY_max.text()):
                y_label.append(float(self.ejeY_max.text()))
            else:
                y_label.append(y[i][pos])
            label.append(config.representacion(isolineas[i]))
            if self.ejeX_escala.isChecked():
                fraccionx=(log(x[i][pos+5])-log(x[i][pos]))/(log(float(self.ejeX_max.text()))-log(float(self.ejeX_min.text())))
            else:
                fraccionx=(x[i][pos+5]-x[i][pos])/(float(self.ejeX_max.text())-float(self.ejeX_min.text()))
            if self.ejeY_escala.isChecked():
                fracciony=(log(y[i][pos+5])-log(y[i][pos]))/(log(float(self.ejeY_max.text()))-log(float(self.ejeY_min.text())))
            else:
                fracciony=(y[i][pos+5]-y[i][pos])/(float(self.ejeY_max.text())-float(self.ejeY_min.text()))
            try:
                angle.append(arctan(fracciony/fraccionx)*360/2/pi)
            except ZeroDivisionError:
                angle.append(90)
        return x_label, y_label, label, angle

    def calcularIsoentropica(self, start=85, rango=5):
        """Método que actualiza los datos de isoentrópicas"""
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Calculando isoentrópicas...", None, QtGui.QApplication.UnicodeUTF8))
        if self.PersonalizarIsoentropica.isChecked():
            S=[]
            if not self.ListaIsoentropica.text().isEmpty():
                S1=self.ListaIsoentropica.text().split(',')
                for i in S1:
                    S.append(unidades.SpecificHeat(float(i)).unit(config.Configuracion("SpecificHeat", "Entropy").func()))
        else:
            S=arange(unidades.SpecificHeat(float(self.S_ini.text())).unit(config.Configuracion("SpecificHeat", "Entropy").func()), unidades.SpecificHeat(float(self.S_fin.text())).unit(config.Configuracion("SpecificHeat", "Entropy").func()), unidades.SpecificHeat(float(self.S_intervalo.text())).unit(config.Configuracion("SpecificHeat", "Entropy").func()))
        if self.IsoentropicaCritica.isChecked():
            SCRIT=steam_pT(PCRIT, TCRIT).s
            S=concatenate((S, [SCRIT]))   
        X= logspace(-3, 3, 100)*1e5
        x, y, z, x2, y2=self.isolineas(S, X, steam_ps, start, rango)
        x_label, y_label, label, angle=self.labels([unidades.SpecificHeat(i).config("Entropy") for i in S], x2, y2, self.PosIsoEntrLabel.value())
        self.isoentropica=[x, y, z]
        self.isoentropica2=[x2, y2, x_label, y_label, ["S="+i+config.Configuracion("SpecificHeat", "Entropy").text() for i in label], angle]

    def calcularIsoentalpica(self, start=90, rango=5):
        """Método que actualiza los datos de isoentálpicas"""
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Calculando isoentálpicas...", None, QtGui.QApplication.UnicodeUTF8))
        if self.PersonalizarIsoentalpica.isChecked():
            H=[]
            if not self.ListaIsoentalpica.text().isEmpty():
                H1=self.ListaIsoentalpica.text().split(',')
                for i in H1:
                    H.append(unidades.Enthalpy(float(i)).unit(config.Configuracion("Enthalpy").func()))
        else:
            H=arange(unidades.Enthalpy(float(self.H_ini.text())).unit(config.Configuracion("Enthalpy").func()), unidades.Enthalpy(float(self.H_fin.text())).unit(config.Configuracion("Enthalpy").func()), unidades.Enthalpy(float(self.H_intervalo.text())).unit(config.Configuracion("Enthalpy").func()))
        if self.IsoentalpicaCritica.isChecked():
            HCRIT=steam_pT(PCRIT, TCRIT).h
            H=concatenate((H, [HCRIT]))
            
        X= logspace(-3, 3, 100)*1e5
        x, y, z, x2, y2=self.isolineas(H, X, steam_ph, start, rango)
        x_label, y_label, label, angle=self.labels([unidades.Enthalpy(i).config for i in H], x2, y2, self.PosIsoEnthLabel.value())
        self.isoentalpica=[x, y, z]
        self.isoentalpica2=[x2, y2, x_label, y_label, ["H="+i+config.Configuracion("Enthalpy").text() for i in label], angle]

    def calcularIsobara(self, start=80, rango=5):
        """Método que actualiza los datos de isobaras"""
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Calculando isobaras...", None, QtGui.QApplication.UnicodeUTF8))
        if self.PersonalizarIsobara.isChecked():
            P=[]
            if not self.ListaIsobara.text().isEmpty():
                P1=self.ListaIsobara.text().split(',')
                for i in P1:
                    P.append(unidades.Pressure(float(i)).unit(config.Configuracion("Pressure").func()))
        else:
            P=arange(unidades.Pressure(float(self.P_ini.text())).unit(config.Configuracion("Pressure").func()), unidades.Pressure(float(self.P_fin.text())).unit(config.Configuracion("Pressure").func()), unidades.Pressure(float(self.P_intervalo.text())).unit(config.Configuracion("Pressure").func()))
        if self.IsobaraCritica.isChecked():
            P=concatenate((P, [PCRIT]))
        X= linspace(273, 1073.15, 100)
        x, y, z, x2, y2=self.isolineas(X, P, steam_pT, start, rango)
        x_label, y_label, label, angle=self.labels([unidades.Pressure(i).config for i in P], transpose(x2), transpose(y2), self.PosIsoBarLabel.value())
        self.isobara=[transpose(x), transpose(y), transpose(z)]
        self.isobara2=[transpose(x2), transpose(y2), x_label, y_label, ["P="+i+config.Configuracion("Pressure").text() for i in label], angle]

    def calcularIsoterma(self, start=75, rango=5):
        """Método que actualiza los datos de isotermas"""
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Calculando isotermas...", None, QtGui.QApplication.UnicodeUTF8))
        if self.PersonalizarIsoterma.isChecked():
            T=[]
            if not self.ListaIsoterma.text().isEmpty():
                T1=self.ListaIsoterma.text().split(',')
                for i in T1:
                    T.append(unidades.Temperature(float(i)).unit(config.Configuracion("Temperature").func()))
        else:
            T=arange(unidades.Temperature(float(self.T_ini.text())).unit(config.Configuracion("Temperature").func()), unidades.Temperature(float(self.T_fin.text())).unit(config.Configuracion("Temperature").func()), unidades.DeltaT(float(self.T_intervalo.text())).unit(config.Configuracion("Temperature").func()))
        if self.IsotermaCritica.isChecked():
            T=concatenate((T, [TCRIT]))
        X= logspace(-3, 3, 100)*1e5
        for i in T:
            if i<TCRIT:
                ps=psat_T(i)
                X=concatenate((X, [ps*0.999, ps*1.001]))
        X.sort()
        x, y, z, x2, y2=self.isolineas(T, X, steam_pT, start, rango)
        x_label, y_label, label, angle=self.labels([unidades.Temperature(i).config for i in T], x2, y2, self.PosIsoThermLabel.value())
        self.isoterma=[x, y, z]
        self.isoterma2=[x2, y2, x_label, y_label, ["T="+i+config.Configuracion("Temperature").text() for i in label], angle]

    def calcularIsocora(self, start=95, rango=5):
        """Método que actualiza los datos de isocoras"""
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Calculando isocoras...", None, QtGui.QApplication.UnicodeUTF8))
        if self.PersonalizarIsocora.isChecked():
            V=[]
            if not self.ListaIsocora.text().isEmpty():
                V1=self.ListaIsocora.text().split(',')
                for i in V1:
                    V.append(unidades.SpecificVolume(float(i)).unit(config.Configuracion("SpecificVolume").func()))
        else:
            V=arange(unidades.SpecificVolume(float(self.V_ini.text())).unit(config.Configuracion("SpecificVolume").func()), unidades.SpecificVolume(float(self.V_fin.text())).unit(config.Configuracion("SpecificVolume").func()), unidades.SpecificVolume(float(self.V_intervalo.text())).unit(config.Configuracion("SpecificVolume").func()))
        if self.IsocoraCritica.isChecked():
            VCRIT=steam_pT(PCRIT, TCRIT).v
            V=concatenate((V, [VCRIT]))
        X= logspace(-3, 3, 100)*1e5
        x, y, z, x2, y2=self.isolineas(V, X, steam_pv, start, rango)
        x2_corr=[]
        y2_corr=[]
        for i in range(len(x2)):
            for j in range(len(x2[i])):
                if bounds_pv(x2[i][j], y2[i][j], 0)==0:
                    x2_corr.append(x2[i])
                    y2_corr.append(y2[i])
        x_label, y_label, label, angle=self.labels([unidades.SpecificVolume(i).config for i in V], x2_corr, y2_corr, self.PosIsoVolLabel.value())
        self.isocora=[x, y, z]        
        self.isocora2=[x2_corr, y2_corr, x_label, y_label, ["v="+i+config.Configuracion("SpecificVolume").text() for i in label], angle]
        
    def calcularIsoX(self, start=95, rango=5):
        """Método que actualiza los datos de isocalidad"""
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Calculando isocalidades...", None, QtGui.QApplication.UnicodeUTF8))
        if self.PersonalizarIsoX.isChecked():
            X=[]
            if not self.ListaIsoX.text().isEmpty():
                X=map(float, self.ListaIsoX.text().split(','))
        else:
            X=arange(float(self.X_ini.text()), float(self.X_fin.text()), float(self.X_intervalo.text()))
        T= linspace(273.15, TCRIT, 100)
        x, y, z, x2, y2=self.isolineas(X, T, steam_Tx, start, rango)
        x_label, y_label, label, angle=self.labels(X, x2, y2, self.PosIsoXLabel.value())
        self.isoX=[x, y, z]        
        self.isoX2=[x2, y2, x_label, y_label, ["x="+i for i in label], angle]


    def dibujar(self):
        """Método que dibuja todos los datos pedidos"""
        self.progresbar.setValue(100)
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Dibujando...", None, QtGui.QApplication.UnicodeUTF8))
        QtGui.QApplication.processEvents()
        self.diagrama3D.plot_3D(self.etiquetas, self.xdata, self.ydata, self.zdata) 
        if len(self.puntos)>0:
            self.diagrama3D.plot_puntos(self.xi, self.yi, self.zi)
        self.diagrama3D.plot_sat(self.xsat, self.ysat, self.zsat)
        self.diagrama3D.plot_Isolinea("T", self.isoterma[0], self.isoterma[1], self.isoterma[2], str(self.IsotermaColor.name()), self.IsotermaGrosor.value(), self.IsotermaLinea.currentIndex())
        self.diagrama3D.plot_Isolinea("P", self.isobara[0], self.isobara[1], self.isobara[2], str(self.IsobaraColor.name()), self.IsobaraGrosor.value(), self.IsobaraLinea.currentIndex())
        self.diagrama3D.plot_Isolinea("V", self.isocora[0], self.isocora[1], self.isocora[2], str(self.IsocoraColor.name()), self.IsocoraGrosor.value(), self.IsocoraLinea.currentIndex())
        self.diagrama3D.plot_Isolinea("H", self.isoentalpica[0], self.isoentalpica[1], self.isoentalpica[2], str(self.IsoentalpicaColor.name()), self.IsoentalpicaGrosor.value(), self.IsoentalpicaLinea.currentIndex())
        self.diagrama3D.plot_Isolinea("S", self.isoentropica[0], self.isoentropica[1], self.isoentropica[2], str(self.IsoentropicaColor.name()), self.IsoentropicaGrosor.value(), self.IsoentropicaLinea.currentIndex())
        self.diagrama3D.plot_Isolinea("X", self.isoX[0], self.isoX[1], self.isoX[2], str(self.IsoXColor.name()), self.IsoXGrosor.value(), self.IsoXLinea.currentIndex())
        self.diagrama3D.axes3D.set_xlim3d(self.xdata[0][0], self.xdata[-1][-1])
        self.diagrama3D.axes3D.set_ylim3d(self.ydata[0][0], self.ydata[-1][-1])
        self.diagrama3D.axes3D.set_zlim3d(min(self.zdata), max(self.zdata))
        self.diagrama3D.draw()
        
        self.diagrama2D.plot_2D(self.etiquetas2, self.rejilla.isChecked())
        self.diagrama2D.plot_sat(self.xsat2, self.ysat2)
        self.diagrama2D.plot_Isolinea("T", self.isoterma2[0], self.isoterma2[1], color=str(self.IsotermaColor.name()), grosor=self.IsotermaGrosor.value(), estilo=self.IsotermaLinea.currentIndex())
        self.diagrama2D.plot_Isolinea("P", self.isobara2[0], self.isobara2[1], self.isobara[2], str(self.IsobaraColor.name()), self.IsobaraGrosor.value(), self.IsobaraLinea.currentIndex())
        self.diagrama2D.plot_Isolinea("V", self.isocora2[0], self.isocora2[1], self.isocora[2], str(self.IsocoraColor.name()), self.IsocoraGrosor.value(), self.IsocoraLinea.currentIndex())
        self.diagrama2D.plot_Isolinea("H", self.isoentalpica2[0], self.isoentalpica2[1], self.isoentalpica[2], str(self.IsoentalpicaColor.name()), self.IsoentalpicaGrosor.value(), self.IsoentalpicaLinea.currentIndex())
        self.diagrama2D.plot_Isolinea("S", self.isoentropica2[0], self.isoentropica2[1], self.isoentropica[2], str(self.IsoentropicaColor.name()), self.IsoentropicaGrosor.value(), self.IsoentropicaLinea.currentIndex())
        self.diagrama2D.plot_Isolinea("X", self.isoX2[0], self.isoX2[1], self.isoX[2], str(self.IsoXColor.name()), self.IsoXGrosor.value(), self.IsoXLinea.currentIndex())
        if len(self.puntos)>0:
            self.diagrama2D.plot_puntos(self.xi2, self.yi2)

        self.diagrama2D.plot_labels("X", self.isoX2[2], self.isoX2[3], self.isoX2[4], self.isoX2[5])
        self.diagrama2D.plot_labels("S", self.isoentropica2[2], self.isoentropica2[3], self.isoentropica2[4], self.isoentropica2[5])
        self.diagrama2D.plot_labels("H", self.isoentalpica2[2], self.isoentalpica2[3], self.isoentalpica2[4], self.isoentalpica2[5])
        self.diagrama2D.plot_labels("V", self.isocora2[2], self.isocora2[3], self.isocora2[4], self.isocora2[5])
        self.diagrama2D.plot_labels("P", self.isobara2[2], self.isobara2[3], self.isobara2[4], self.isobara2[5])
        self.diagrama2D.plot_labels("T", self.isoterma2[2], self.isoterma2[3], self.isoterma2[4], self.isoterma2[5])

#        self.mostrarLabels()
        self.mostrarSaturacion(self.actionDibujarSaturacion.isChecked())
        self.mostrarPuntos(self.actionMostrarPuntos.isChecked())
        self.mostrarIsoentropica()
        self.mostrarIsocora()
        self.mostrarIsoentalpica()
        self.mostrarIsobara()
        self.mostrarIsoterma()
        self.mostrarIsoX()
        self.mostrarPuntos(self.actionMostrarPuntos.isChecked())
        self.diagrama2D_ejeX()
        self.diagrama2D_ejeY()
        self.ejeX_log(self.ejeX_escala.isChecked())
        self.ejeY_log(self.ejeY_escala.isChecked())
        self.progresbar.setVisible(False)
        self.statusbar.showMessage(QtGui.QApplication.translate("SteamTables", "Listo...", None, QtGui.QApplication.UnicodeUTF8))

    def rejilla_toggled(self, bool):
        """Muestra o esconde la rejilla del gráfico 2D"""
        self.diagrama2D.axes2D.grid(bool)
        self.diagrama2D.draw()
        
    def diagrama2D_ejeX(self):
        """Define la orientación del eje x, creciente o decreciente"""
        if self.ejeX_min.text()<>"" and self.ejeX_max.text()<>"":
            xmin=float(self.ejeX_min.text())
            xmax=float(self.ejeX_max.text())
            self.diagrama2D.axes2D.set_xlim(xmin, xmax)
            self.diagrama2D.axes2D.set_autoscalex_on(False)
        else:
            self.diagrama2D.axes2D.set_autoscalex_on(True)
        self.diagrama2D.draw()

    def diagrama2D_ejeY(self):
        """Define la orientación del eje y, creciente o decreciente"""
        if self.ejeY_min.text()<>"" and self.ejeY_max.text()<>"":
            ymin=float(self.ejeY_min.text())
            ymax=float(self.ejeY_max.text())
            self.diagrama2D.axes2D.set_ylim(ymin, ymax)
            self.diagrama2D.axes2D.set_autoscaley_on(False)
        else:
            self.diagrama2D.axes2D.set_autoscaley_on(True)
        self.diagrama2D.draw()
        
    def ejeX_log(self, bool):
        """Define la escala del eje x, normal o logarítmica"""
        if bool:
            self.diagrama2D.axes2D.set_xscale("log")
        else:
            self.diagrama2D.axes2D.set_xscale("linear")
        self.diagrama2D.draw()

    def ejeY_log(self, bool):
        """Define la escala del eje y, normal o logarítmica"""
        if bool:
            self.diagrama2D.axes2D.set_yscale("log")
        else:
            self.diagrama2D.axes2D.set_yscale("linear")
        self.diagrama2D.draw()

    
    def botonAdd_clicked(self):
        """Añade el punto especificado actual a la lista"""
        self.puntos.append(self.punto)
        self.calcularPuntos()
        self.dibujar()
    
    def botonLista_clicked(self):
        """Muestra la lista de puntos especificados por el usuario"""
        dialog=Ventana_Lista_Puntos()
        dialog.setupUi(dialog, self.puntos)
        if dialog.exec_():
            self.puntos=dialog.puntos
            self.calcularPuntos()
            self.dibujar()

    def botonActualizar_clicked(self):
        """Actualiza las isolineas dibujadas en los gráficos"""
        self.progresbar.setVisible(True)
        self.calcularIsoterma(0, 15)
        self.calcularIsobara(15, 15)
        self.calcularIsoentropica(30, 15)
        self.calcularIsoentalpica(45, 15)
        self.calcularIsocora(60, 15)
        self.calcularIsoX(75, 25)
        self.dibujar()

if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    locale = QtCore.QLocale.system().name()
    qtTranslator = QtCore.QTranslator()
    if qtTranslator.load("UI_steamTables_" + locale, "i18n"):
        app.installTranslator(qtTranslator)
    SteamTables= Ui_SteamTables()
    SteamTables.setupUi(SteamTables)
    SteamTables.show()
    sys.exit(app.exec_())
