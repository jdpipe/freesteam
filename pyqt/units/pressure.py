#!/usr/bin/python
# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui

from unidades import Pressure
from config import representacion

class Ui_presion(QtGui.QDialog):
    def setupUi(self, presion):
        self.semaforo=QtCore.QSemaphore(1)
        presion.resize(350, presion.minimumHeight())
        self.gridLayout = QtGui.QGridLayout(presion)
        
        self.label = QtGui.QLabel(presion)
        self.label.setAlignment(QtCore.Qt.AlignRight)
        self.label.setText("bar")
        self.gridLayout.addWidget(self.label,1,1,1,1)
        self.label_6 = QtGui.QLabel(presion)
        self.label_6.setAlignment(QtCore.Qt.AlignRight)
        self.label_6.setText("mbar")
        self.gridLayout.addWidget(self.label_6,2,1,1,1)
        self.label_2 = QtGui.QLabel(presion)
        self.label_2.setAlignment(QtCore.Qt.AlignRight)
        self.label_2.setText("atm")
        self.gridLayout.addWidget(self.label_2,3,1,1,1)
        self.label_3 = QtGui.QLabel(presion)
        self.label_3.setAlignment(QtCore.Qt.AlignRight)
        self.label_3.setText("Pa")
        self.gridLayout.addWidget(self.label_3,4,1,1,1)
        self.label_4 = QtGui.QLabel(presion)
        self.label_4.setAlignment(QtCore.Qt.AlignRight)
        self.label_4.setText("kPa")
        self.gridLayout.addWidget(self.label_4,5,1,1,1)
        self.label_5 = QtGui.QLabel(presion)
        self.label_5.setAlignment(QtCore.Qt.AlignRight)
        self.label_5.setText("MPa")
        self.gridLayout.addWidget(self.label_5,6,1,1,1)
        self.label_10 = QtGui.QLabel(presion)
        self.label_10.setAlignment(QtCore.Qt.AlignRight)
        self.label_10.setText("""<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">\n<html><head><meta name="qrichtext" content="1" /><style type="text/css">\np, li { white-space: pre-wrap; }\n</style></head><body style=" font-family:'Nimbus Sans L'; font-size:9pt; font-weight:400; font-style:normal;">\n<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">mm H<span style=" vertical-align:sub;">2</span>O</p></body></html>""")
        self.gridLayout.addWidget(self.label_10,7,1,1,1)
        self.label_7 = QtGui.QLabel(presion)
        self.label_7.setAlignment(QtCore.Qt.AlignRight)
        self.label_7.setText("""<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">\n<html><head><meta name="qrichtext" content="1" /><style type="text/css">\np, li { white-space: pre-wrap; }\n</style></head><body style=" font-family:'Nimbus Sans L'; font-size:9pt; font-weight:400; font-style:normal;">\n<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">cm H<span style=" vertical-align:sub;">2</span>O</p></body></html>""")
        self.gridLayout.addWidget(self.label_7,8,1,1,1)
        self.label_13 = QtGui.QLabel(presion)
        self.label_13.setAlignment(QtCore.Qt.AlignRight)
        self.label_13.setText("mm Hg")
        self.gridLayout.addWidget(self.label_13,9,1,1,1)

        self.label_12 = QtGui.QLabel(presion)
        self.label_12.setAlignment(QtCore.Qt.AlignRight)
        self.label_12.setText("torr")
        self.gridLayout.addWidget(self.label_12,1,4,1,1)
        self.label_8 = QtGui.QLabel(presion)
        self.label_8.setAlignment(QtCore.Qt.AlignRight)
        self.label_8.setText(u"kg/cm²")
        self.gridLayout.addWidget(self.label_8,2,4,1,1)
        self.label_9 = QtGui.QLabel(presion)
        self.label_9.setAlignment(QtCore.Qt.AlignRight)
        self.label_9.setText("psi")
        self.gridLayout.addWidget(self.label_9,3,4,1,1)
        self.label_16 = QtGui.QLabel(presion)
        self.label_16.setAlignment(QtCore.Qt.AlignRight)
        self.label_16.setText("psig")
        self.gridLayout.addWidget(self.label_16,4,4,1,1)
        self.label_11 = QtGui.QLabel(presion)
        self.label_11.setAlignment(QtCore.Qt.AlignRight)
        self.label_11.setText("bar G")
        self.gridLayout.addWidget(self.label_11,5,4,1,1)
        self.label_14 = QtGui.QLabel(presion)
        self.label_14.setAlignment(QtCore.Qt.AlignRight)
        self.label_14.setText(u"kg/cm² G")
        self.gridLayout.addWidget(self.label_14,6,4,1,1)
        self.label_18 = QtGui.QLabel(presion)
        self.label_18.setAlignment(QtCore.Qt.AlignRight)
        self.label_18.setText("""<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">\n<html><head><meta name="qrichtext" content="1" /><style type="text/css">\np, li { white-space: pre-wrap; }\n</style></head><body style=" font-family:'Nimbus Sans L'; font-size:9pt; font-weight:400; font-style:normal;">\n<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">ft H<span style=" vertical-align:sub;">2</span>O</p></body></html>""")
        self.gridLayout.addWidget(self.label_18,7,4,1,1) 
        self.label_17 = QtGui.QLabel(presion)
        self.label_17.setAlignment(QtCore.Qt.AlignRight)
        self.label_17.setText("""<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">\n<html><head><meta name="qrichtext" content="1" /><style type="text/css">\np, li { white-space: pre-wrap; }\n</style></head><body style=" font-family:'Nimbus Sans L'; font-size:9pt; font-weight:400; font-style:normal;">\n<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">in H<span style=" vertical-align:sub;">2</span>O</p></body></html>""")
        self.gridLayout.addWidget(self.label_17,8,4,1,1)
        self.label_15 = QtGui.QLabel(presion)
        self.label_15.setAlignment(QtCore.Qt.AlignRight)
        self.label_15.setText("in Hg")
        self.gridLayout.addWidget(self.label_15,9,4,1,1)

        self.bar = QtGui.QLineEdit(presion)
        self.bar.setAlignment(QtCore.Qt.AlignRight)
        self.bar.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.bar,1,2,1,1)
        self.mbar = QtGui.QLineEdit(presion)
        self.mbar.setAlignment(QtCore.Qt.AlignRight)
        self.mbar.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.mbar,2,2,1,1)
        self.atm = QtGui.QLineEdit(presion)
        self.atm.setAlignment(QtCore.Qt.AlignRight)
        self.atm.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.atm,3,2,1,1)
        self.Pa = QtGui.QLineEdit(presion)
        self.Pa.setAlignment(QtCore.Qt.AlignRight)
        self.Pa.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.Pa,4,2,1,1)
        self.kPa = QtGui.QLineEdit(presion)
        self.kPa.setAlignment(QtCore.Qt.AlignRight)
        self.kPa.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.kPa,5,2,1,1)
        self.MPa = QtGui.QLineEdit(presion)
        self.MPa.setAlignment(QtCore.Qt.AlignRight)
        self.MPa.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.MPa,6,2,1,1)
        self.mmH2O = QtGui.QLineEdit(presion)
        self.mmH2O.setAlignment(QtCore.Qt.AlignRight)
        self.mmH2O.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.mmH2O,7,2,1,1)
        self.cmH2O = QtGui.QLineEdit(presion)
        self.cmH2O.setAlignment(QtCore.Qt.AlignRight)
        self.cmH2O.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.cmH2O,8,2,1,1)
        self.mmHg = QtGui.QLineEdit(presion)
        self.mmHg.setAlignment(QtCore.Qt.AlignRight)
        self.mmHg.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.mmHg,9,2,1,1)

        self.torr = QtGui.QLineEdit(presion)
        self.torr.setAlignment(QtCore.Qt.AlignRight)
        self.torr.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.torr,1,5,1,1)
        self.kgcm2 = QtGui.QLineEdit(presion)
        self.kgcm2.setAlignment(QtCore.Qt.AlignRight)
        self.kgcm2.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.kgcm2,2,5,1,1)
        self.psi = QtGui.QLineEdit(presion)
        self.psi.setAlignment(QtCore.Qt.AlignRight)
        self.psi.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.psi,3,5,1,1)
        self.psig = QtGui.QLineEdit(presion)
        self.psig.setAlignment(QtCore.Qt.AlignRight)
        self.psig.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.psig,4,5,1,1)
        self.barg = QtGui.QLineEdit(presion)
        self.barg.setAlignment(QtCore.Qt.AlignRight)
        self.barg.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.barg,5,5,1,1)
        self.kgcm2g = QtGui.QLineEdit(presion)
        self.kgcm2g.setAlignment(QtCore.Qt.AlignRight)
        self.kgcm2g.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.kgcm2g,6,5,1,1)
        self.ftH2O = QtGui.QLineEdit(presion)
        self.ftH2O.setAlignment(QtCore.Qt.AlignRight)
        self.ftH2O.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.ftH2O,7,5,1,1)
        self.inH2O = QtGui.QLineEdit(presion)
        self.inH2O.setAlignment(QtCore.Qt.AlignRight)
        self.inH2O.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.inH2O,8,5,1,1)
        self.inHg = QtGui.QLineEdit(presion)
        self.inHg.setAlignment(QtCore.Qt.AlignRight)
        self.inHg.setValidator(QtGui.QDoubleValidator(presion))
        self.gridLayout.addWidget(self.inHg,9,5,1,1)

        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,0,10,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,0,1,6)
        self.gridLayout.addItem(QtGui.QSpacerItem(10,10,QtGui.QSizePolicy.Fixed,QtGui.QSizePolicy.Fixed),0,3,6,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,6,10,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),10,0,1,6)

        
        self.buttonBox = QtGui.QDialogButtonBox(presion)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setCenterButtons(True)        
        self.gridLayout.addWidget(self.buttonBox,11,1,1,5)
        
        self.retranslateUi(presion)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("accepted()"),presion.accept)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("rejected()"),presion.reject)
        QtCore.QObject.connect(self.bar,QtCore.SIGNAL("editingFinished()"),self.actualizar_bar)        
        QtCore.QObject.connect(self.mbar,QtCore.SIGNAL("editingFinished()"),self.actualizar_mbar)    
        QtCore.QObject.connect(self.atm,QtCore.SIGNAL("editingFinished()"),self.actualizar_atm) 
        QtCore.QObject.connect(self.Pa,QtCore.SIGNAL("editingFinished()"),self.actualizar_Pa) 
        QtCore.QObject.connect(self.kPa,QtCore.SIGNAL("editingFinished()"),self.actualizar_kPa)   
        QtCore.QObject.connect(self.MPa,QtCore.SIGNAL("editingFinished()"),self.actualizar_MPa)
        QtCore.QObject.connect(self.mmH2O,QtCore.SIGNAL("editingFinished()"),self.actualizar_mmH2O)
        QtCore.QObject.connect(self.cmH2O,QtCore.SIGNAL("editingFinished()"),self.actualizar_cmH2O)
        QtCore.QObject.connect(self.mmHg,QtCore.SIGNAL("editingFinished()"),self.actualizar_mmHg)
        QtCore.QObject.connect(self.torr,QtCore.SIGNAL("editingFinished()"),self.actualizar_torr)
        QtCore.QObject.connect(self.kgcm2,QtCore.SIGNAL("editingFinished()"),self.actualizar_kgcm2)
        QtCore.QObject.connect(self.psi,QtCore.SIGNAL("editingFinished()"),self.actualizar_psi)
        QtCore.QObject.connect(self.psig,QtCore.SIGNAL("editingFinished()"),self.actualizar_psig)
        QtCore.QObject.connect(self.barg,QtCore.SIGNAL("editingFinished()"),self.actualizar_barg)
        QtCore.QObject.connect(self.kgcm2g,QtCore.SIGNAL("editingFinished()"),self.actualizar_kgcm2g)
        QtCore.QObject.connect(self.ftH2O,QtCore.SIGNAL("editingFinished()"),self.actualizar_ftH2O)
        QtCore.QObject.connect(self.inHg,QtCore.SIGNAL("editingFinished()"),self.actualizar_inHg)
        QtCore.QObject.connect(self.inH2O,QtCore.SIGNAL("editingFinished()"),self.actualizar_inH2O)
        
    def retranslateUi(self, presion):
        presion.setWindowTitle(QtGui.QApplication.translate("presion", "Presión", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setWhatsThis(QtGui.QApplication.translate("presion", 'Se denomina bar a una unidad de presión equivalente a un millón de barias, aproximadamente igual a una atmósfera (1 Atm). Su símbolo es "bar". La palabra bar tiene su origen en báros, que en griego significa peso', None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setWhatsThis(QtGui.QApplication.translate("presion", "La unidad de presión denominada atmósfera equivale a la presión de la atmósfera terrestre sobre el nivel del mar. Es utilizada para medir presiones elevadas como la de los gases comprimidos.", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setWhatsThis(QtGui.QApplication.translate("presion", "El pascal (símbolo Pa) es la unidad de presión del Sistema Internacional de Unidades. Se define como la presión que ejerce una fuerza de 1 newton sobre una superficie de 1 metro cuadrado normal a la misma", None, QtGui.QApplication.UnicodeUTF8))
        self.label_7.setWhatsThis(QtGui.QApplication.translate("presion", "Presión ejercida por una columna de agua de altura dada en centímetros", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setWhatsThis(QtGui.QApplication.translate("presion", "La libra-fuerza por pulgada cuadrada, más conocida como psi (del inglés pounds per square inch) es una unidad de presión en el sistema anglosajón de unidades. Equivale a 6894,75 pascales", None, QtGui.QApplication.UnicodeUTF8))
        self.label_10.setWhatsThis(QtGui.QApplication.translate("presion", "Presión ejercida por una columna de agua de altura dada en milímetros", None, QtGui.QApplication.UnicodeUTF8))
        self.label_11.setWhatsThis(QtGui.QApplication.translate("presion", "Presión en bares por encima de la presión atmosférica", None, QtGui.QApplication.UnicodeUTF8))
        self.label_12.setWhatsThis(QtGui.QApplication.translate("presion", "El torr es una unidad de presión, denominada así en honor de Evangelista Torricelli. Se define como la presión ejercida en la base de una columna de un milímetro de mercurio, cuya densidad es de 13,5951 g/cm³ en un lugar donde la aceleración de la gravedad es de 9,80665 m/s³ y como 1/760 de atmósfera.", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setWhatsThis(QtGui.QApplication.translate("presion", "Presión ejercida por una columna de mercurio de altura dada en milimetros", None, QtGui.QApplication.UnicodeUTF8))
        self.label_14.setWhatsThis(QtGui.QApplication.translate("presion", "Presión en kg por centimetro cuadrado por encima de la presión atmosférica", None, QtGui.QApplication.UnicodeUTF8))
        self.label_15.setWhatsThis(QtGui.QApplication.translate("presion", "Presión ejercida por una columna de mercurio de altura dada en pulgadas", None, QtGui.QApplication.UnicodeUTF8))
        self.label_16.setWhatsThis(QtGui.QApplication.translate("presion", "Presion en libras por pulgada cuadrada por encima de la presión atmosférica", None, QtGui.QApplication.UnicodeUTF8))
        self.label_17.setWhatsThis(QtGui.QApplication.translate("presion", "Presión ejercida por una columna de agua de altura dada en pulgadas", None, QtGui.QApplication.UnicodeUTF8))
        self.label_18.setWhatsThis(QtGui.QApplication.translate("presion", "Presión ejercida por una columna de agua de altura dada en pies", None, QtGui.QApplication.UnicodeUTF8))
        self.bar.setToolTip(QtGui.QApplication.translate("presion", "bar", None, QtGui.QApplication.UnicodeUTF8))
        self.mbar.setToolTip(QtGui.QApplication.translate("presion", "milibar", None, QtGui.QApplication.UnicodeUTF8))
        self.atm.setToolTip(QtGui.QApplication.translate("presion", "atmósfera", None, QtGui.QApplication.UnicodeUTF8))
        self.Pa.setToolTip(QtGui.QApplication.translate("presion", "Pascal", None, QtGui.QApplication.UnicodeUTF8))
        self.kPa.setToolTip(QtGui.QApplication.translate("presion", "kilopascal", None, QtGui.QApplication.UnicodeUTF8))
        self.MPa.setToolTip(QtGui.QApplication.translate("presion", "megapascal", None, QtGui.QApplication.UnicodeUTF8))
        self.mmH2O.setToolTip(QtGui.QApplication.translate("presion", "milimetro de agua", None, QtGui.QApplication.UnicodeUTF8))
        self.cmH2O.setToolTip(QtGui.QApplication.translate("presion", "centímetro de agua", None, QtGui.QApplication.UnicodeUTF8))
        self.mmHg.setToolTip(QtGui.QApplication.translate("presion", "milimetro de mercurio", None, QtGui.QApplication.UnicodeUTF8))
        self.torr.setToolTip(QtGui.QApplication.translate("presion", "torr", None, QtGui.QApplication.UnicodeUTF8))
        self.kgcm2.setToolTip(QtGui.QApplication.translate("presion", "kilogramo fuerza por centímetro cuadrado", None, QtGui.QApplication.UnicodeUTF8))
        self.psi.setToolTip(QtGui.QApplication.translate("presion", "libra por pulgada cudrada absoluta", None, QtGui.QApplication.UnicodeUTF8))
        self.psig.setToolTip(QtGui.QApplication.translate("presion", "libra por pulgada cuadrada por encima de la presión atmosférica", None, QtGui.QApplication.UnicodeUTF8))
        self.barg.setToolTip(QtGui.QApplication.translate("presion", "bar por encima de la presión atmosférica", None, QtGui.QApplication.UnicodeUTF8))
        self.kgcm2g.setToolTip(QtGui.QApplication.translate("presion", "kilogramo por centímetro cuadrado por encima de la presión atmosférica", None, QtGui.QApplication.UnicodeUTF8))
        self.ftH2O.setToolTip(QtGui.QApplication.translate("presion", "pie de agua", None, QtGui.QApplication.UnicodeUTF8))
        self.inH2O.setToolTip(QtGui.QApplication.translate("presion", "pulgada de agua", None, QtGui.QApplication.UnicodeUTF8))
        self.inHg.setToolTip(QtGui.QApplication.translate("presion", "pulgada de mercurio", None, QtGui.QApplication.UnicodeUTF8))

    def actualizar_bar(self):
        valido=True
        if len(self.bar.text())!=0:
            try:
                self.bar.setText(str(float(self.bar.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.bar.setFocus()
                self.bar.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.bar.text())).unit("bar")
                self.actualizar()

    def actualizar_mbar(self):
        valido=True
        if len(self.mbar.text())!=0:
            try:
                self.mbar.setText(str(float(self.mbar.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.mbar.setFocus()
                self.mbar.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.mbar.text())).unit("mbar")
                self.actualizar()
            
    def actualizar_atm(self):
        valido=True
        if len(self.atm.text())!=0:
            try:
                self.atm.setText(str(float(self.atm.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.atm.setFocus()
                self.atm.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.atm.text())).unit("atm")
                self.actualizar()
        
    def actualizar_Pa(self):
        valido=True
        if len(self.Pa.text())!=0:
            try:
                self.Pa.setText(str(float(self.Pa.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.Pa.setFocus()
                self.Pa.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.Pa.text()))
                self.actualizar()
            
    def actualizar_kPa(self):
        valido=True
        if len(self.kPa.text())!=0:
            try:
                self.kPa.setText(str(float(self.kPa.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.kPa.setFocus()
                self.kPa.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.kPa.text())).unit("kPa")
                self.actualizar()
    
    def actualizar_MPa(self):
        valido=True
        if len(self.MPa.text())!=0:
            try:
                self.MPa.setText(str(float(self.MPa.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.MPa.setFocus()
                self.MPa.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.MPa.text())).unit("MPa")
                self.actualizar()

    def actualizar_mmH2O(self):
        valido=True
        if len(self.mmH2O.text())!=0:
            try:
                self.mmH2O.setText(str(float(self.mmH2O.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.mmH2O.setFocus()
                self.mmH2O.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.mmH2O.text())).unit("mmH2O")
                self.actualizar()

    def actualizar_cmH2O(self):
        valido=True
        if len(self.cmH2O.text())!=0:
            try:
                self.cmH2O.setText(str(float(self.cmH2O.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.cmH2O.setFocus()
                self.cmH2O.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.cmH2O.text())).unit("cmH2O")
                self.actualizar()

    def actualizar_mmHg(self):
        valido=True
        if len(self.mmHg.text())!=0:
            try:
                self.mmHg.setText(str(float(self.mmHg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.mmHg.setFocus()
                self.mmHg.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.mmHg.text())).unit("mmHg")
                self.actualizar()
        
    def actualizar_torr(self):
        valido=True
        if len(self.torr.text())!=0:
            try:
                self.torr.setText(str(float(self.torr.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.torr.setFocus()
                self.torr.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.torr.text())).unit("torr")
                self.actualizar()
    
    def actualizar_kgcm2(self):
        valido=True
        if len(self.kgcm2.text())!=0:
            try:
                self.kgcm2.setText(str(float(self.kgcm2.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.kgcm2.setFocus()
                self.kgcm2.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.kgcm2.text())).unit("kgcm2")
                self.actualizar()
    
    def actualizar_psi(self):
        valido=True
        if len(self.psi.text())!=0:
            try:
                self.psi.setText(str(float(self.psi.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.psi.setFocus()
                self.psi.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.psi.text())).unit("psi")
                self.actualizar()
    
    def actualizar_psig(self):
        valido=True
        if len(self.psig.text())!=0:
            try:
                self.psig.setText(str(float(self.psig.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.psig.setFocus()
                self.psig.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.psig.text())).unit("psig")
                self.actualizar()
    
    def actualizar_barg(self):
        valido=True
        if len(self.barg.text())!=0:
            try:
                self.barg.setText(str(float(self.barg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.barg.setFocus()
                self.barg.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.barg.text())).unit("barg")
                self.actualizar()

    def actualizar_kgcm2g(self):
        valido=True
        if len(self.kgcm2g.text())!=0:
            try:
                self.kgcm2g.setText(str(float(self.kgcm2g.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.kgcm2g.setFocus()
                self.kgcm2g.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.kgcm2g.text())).unit("kgcm2g")
                self.actualizar()
        
    def actualizar_ftH2O(self):
        valido=True
        if len(self.ftH2O.text())!=0:
            try:
                self.ftH2O.setText(str(float(self.ftH2O.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.ftH2O.setFocus()
                self.ftH2O.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.ftH2O.text())).unit("ftH2O")
                self.actualizar()
    
    def actualizar_inHg(self):
        valido=True
        if len(self.inHg.text())!=0:
            try:
                self.inHg.setText(str(float(self.inHg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.inHg.setFocus()
                self.inHg.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.inHg.text())).unit("inHg")
                self.actualizar()

    def actualizar_inH2O(self):
        valido=True
        if len(self.inH2O.text())!=0:
            try:
                self.inH2O.setText(str(float(self.inH2O.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("presion", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("presion", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.inH2O.setFocus()
                self.inH2O.selectAll()
                valido=False
            if valido:
                self.dato=Pressure(float(self.inH2O.text())).unit("inH2O")
                self.actualizar()

    def actualizar(self):
        if self.semaforo.available()>0:
            self.semaforo.acquire(1)
            self.bar.setText(representacion(self.dato.bar))
            self.mbar.setText(representacion(self.dato.mbar))
            self.Pa.setText(representacion(self.dato.Pa))
            self.kPa.setText(representacion(self.dato.kPa))
            self.MPa.setText(representacion(self.dato.MPa))
            self.atm.setText(representacion(self.dato.atm))
            self.mmH2O.setText(representacion(self.dato.mmH2O))
            self.cmH2O.setText(representacion(self.dato.cmH2O))
            self.mmHg.setText(representacion(self.dato.mmHg))
            self.torr.setText(representacion(self.dato.mmHg))
            self.kgcm2.setText(representacion(self.dato.kgcm2))
            self.psi.setText(representacion(self.dato.psi))
            self.psig.setText(representacion(self.dato.psig))
            self.barg.setText(representacion(self.dato.barg))
            self.kgcm2g.setText(representacion(self.dato.kgcm2g))
            self.ftH2O.setText(representacion(self.dato.ftH2O))
            self.inHg.setText(representacion(self.dato.inHg))
            self.inH2O.setText(representacion(self.dato.inH2O))
            self.semaforo.release(1)

if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    presion = Ui_presion()
    presion.setupUi(presion)
    presion.show()
    sys.exit(app.exec_())
