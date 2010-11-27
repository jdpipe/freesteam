#!/usr/bin/python
# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui

from unidades import Temperature
from config import representacion

class Ui_temperatura(QtGui.QDialog):
    def setupUi(self, Temperatura):
        Temperatura.resize(Temperatura.minimumSize())
        self.semaforo=QtCore.QSemaphore(1)
        self.gridLayout = QtGui.QGridLayout(Temperatura)
        self.label = QtGui.QLabel(Temperatura)
        self.label.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label.setText("K")
        self.gridLayout.addWidget(self.label,1,1,1,1)
        self.label.setMinimumWidth(30)
        self.label_2 = QtGui.QLabel(Temperatura)
        self.label_2.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_2.setText(u"ºC")
        self.gridLayout.addWidget(self.label_2,2,1,1,1)
        self.label_2.setMinimumWidth(30)
        self.label_3 = QtGui.QLabel(Temperatura)
        self.label_3.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_3.setText(u"ºF")
        self.gridLayout.addWidget(self.label_3,3,1,1,1)
        self.label_3.setMinimumWidth(30)
        self.label_4 = QtGui.QLabel(Temperatura)
        self.label_4.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_4.setText(u"ºR")
        self.gridLayout.addWidget(self.label_4,4,1,1,1)
        self.label_4.setMinimumWidth(30)
        self.label_5 = QtGui.QLabel(Temperatura)
        self.label_5.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignVCenter)
        self.label_5.setText(u"ºRe")
        self.gridLayout.addWidget(self.label_5,5,1,1,1)
        self.label_5.setMinimumWidth(30)
        self.kelvin = QtGui.QLineEdit(Temperatura)
        self.kelvin.setAlignment(QtCore.Qt.AlignRight)
        self.kelvin.setValidator(QtGui.QDoubleValidator(Temperatura))
        self.gridLayout.addWidget(self.kelvin,1,2,1,1)
        self.celsius = QtGui.QLineEdit(Temperatura)
        self.celsius.setAlignment(QtCore.Qt.AlignRight)
        self.celsius.setValidator(QtGui.QDoubleValidator(Temperatura))
        self.gridLayout.addWidget(self.celsius,2,2,1,1)
        self.fahrenheit = QtGui.QLineEdit(Temperatura)
        self.fahrenheit.setAlignment(QtCore.Qt.AlignRight)
        self.fahrenheit.setValidator(QtGui.QDoubleValidator(Temperatura))
        self.gridLayout.addWidget(self.fahrenheit,3,2,1,1)
        self.rankine = QtGui.QLineEdit(Temperatura)
        self.rankine.setAlignment(QtCore.Qt.AlignRight)
        self.rankine.setValidator(QtGui.QDoubleValidator(Temperatura))
        self.gridLayout.addWidget(self.rankine,4,2,1,1)
        self.reaumur = QtGui.QLineEdit(Temperatura)
        self.reaumur.setAlignment(QtCore.Qt.AlignRight)
        self.reaumur.setValidator(QtGui.QDoubleValidator(Temperatura))
        self.gridLayout.addWidget(self.reaumur,5,2,1,1)
        
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,0,6,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,0,1,4)
        self.gridLayout.addItem(QtGui.QSpacerItem(30,30,QtGui.QSizePolicy.Fixed,QtGui.QSizePolicy.Fixed),0,3,6,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,3,6,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),6,0,1,4)
        
        self.buttonBox = QtGui.QDialogButtonBox(Temperatura)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setCenterButtons(True)        
        self.gridLayout.addWidget(self.buttonBox,7,1,1,3)
        
        self.retranslateUi(Temperatura)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("accepted()"),Temperatura.accept)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("rejected()"),Temperatura.reject)
        QtCore.QObject.connect(self.kelvin,QtCore.SIGNAL("editingFinished()"),self.actualizar_k)
        QtCore.QObject.connect(self.celsius,QtCore.SIGNAL("editingFinished()"),self.actualizar_c)
        QtCore.QObject.connect(self.fahrenheit,QtCore.SIGNAL("editingFinished()"),self.actualizar_f)
        QtCore.QObject.connect(self.rankine,QtCore.SIGNAL("editingFinished()"),self.actualizar_r)
        QtCore.QObject.connect(self.reaumur,QtCore.SIGNAL("editingFinished()"),self.actualizar_re)

    def retranslateUi(self, Temperatura):
        Temperatura.setWindowTitle(QtGui.QApplication.translate("Temperatura", "Temperatura", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setToolTip(QtGui.QApplication.translate("Temperatura", "Kelvin", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setWhatsThis(QtGui.QApplication.translate("Temperatura", "Denominada así en honor del matemático y físico inglés William Thomson Kelvin (1824-1907). Es la unidad básica de la temperatura en termodinámica y en el SI de unidades.", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setToolTip(QtGui.QApplication.translate("Temperatura", "Celsius", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setWhatsThis(QtGui.QApplication.translate("Temperatura", "Denominada así en honor del astrónomo y físico sueco Anders Celsius (1701-1744). La escala Celsius, también denominado centigrada, marca 0º en el punto de fusión del agua y 100º en su punto de ebullición.", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setWhatsThis(QtGui.QApplication.translate("Temperatura", "Denominada así en honor del físico alemán Gabriel Daniel Fahrenheit (1686-1736). El termómetro Fahrenheit está graduado de tal forma que el punto de congelación es 32º, y el punto de ebullición 212º. Es de uso común en Estados Unidos y Reino Unido así como en ingnierías.", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setToolTip(QtGui.QApplication.translate("Temperatura", "Rankine", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setWhatsThis(QtGui.QApplication.translate("Temperatura", "Denominada así en honor del físico e ingeniero británico William Rankine (1820-1872). Una escala de temperatura absoluta en grados Fahrenheit.", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setToolTip(QtGui.QApplication.translate("Temperatura", "Reaumur", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setWhatsThis(QtGui.QApplication.translate("Temperatura", "Denominados así en honor del científico frances René-Antoine Ferchault de Réaumur (1683-1757) de acuerdo con una escala creada por él según la cual 0º marca el punto de fusión y 80º el punto de ebullición del agua.", None, QtGui.QApplication.UnicodeUTF8))
        self.kelvin.setToolTip(QtGui.QApplication.translate("Temperatura", "Kelvin", None, QtGui.QApplication.UnicodeUTF8))
        self.kelvin.setWhatsThis(QtGui.QApplication.translate("Temperatura", "Denominada así en honor del matemático y físico inglés William Thomson Kelvin (1824-1907). Es la unidad básica de la temperatura en termodinámica y en el SI de unidades.", None, QtGui.QApplication.UnicodeUTF8))
        self.celsius.setToolTip(QtGui.QApplication.translate("Temperatura", "Celsius", None, QtGui.QApplication.UnicodeUTF8))
        self.celsius.setWhatsThis(QtGui.QApplication.translate("Temperatura", "Denominada así en honor del astrónomo y físico sueco Anders Celsius (1701-1744). La escala Celsius, también denominado centigrada, marca 0º en el punto de fusión del agua y 100º en su punto de ebullición.", None, QtGui.QApplication.UnicodeUTF8))
        self.fahrenheit.setToolTip(QtGui.QApplication.translate("Temperatura", "Fahrenheit", None, QtGui.QApplication.UnicodeUTF8))
        self.fahrenheit.setWhatsThis(QtGui.QApplication.translate("Temperatura", "Denominada así en honor del físico alemán Gabriel Daniel Fahrenheit (1686-1736). El termómetro Fahrenheit está graduado de tal forma que el punto de congelación es 32º, y el punto de ebullición 212º. Es de uso común en Estados Unidos y Reino Unido así como en ingnierías.", None, QtGui.QApplication.UnicodeUTF8))
        self.rankine.setToolTip(QtGui.QApplication.translate("Temperatura", "Rankine", None, QtGui.QApplication.UnicodeUTF8))
        self.rankine.setWhatsThis(QtGui.QApplication.translate("Temperatura", "Denominada así en honor del físico e ingeniero británico William Rankine (1820-1872). Una escala de temperatura absoluta en grados Fahrenheit.", None, QtGui.QApplication.UnicodeUTF8))
        self.reaumur.setToolTip(QtGui.QApplication.translate("Temperatura", "Reaumur", None, QtGui.QApplication.UnicodeUTF8))
        self.reaumur.setWhatsThis(QtGui.QApplication.translate("Temperatura", "Denominados así en honor del científico frances René-Antoine Ferchault de Réaumur (1683-1757) de acuerdo con una escala creada por él según la cual 0º marca el punto de fusión y 80º el punto de ebullición del agua.", None, QtGui.QApplication.UnicodeUTF8))


    def actualizar_k(self):
        valido=True
        if len(self.kelvin.text())!=0:
            try:
                self.kelvin.setText(str(float(self.kelvin.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("Temperatura", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("Temperatura", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.kelvin.setFocus()
                self.kelvin.selectAll()
                valido=False
            if valido:
                self.dato=Temperature(float(self.kelvin.text()))
                self.actualizar()

    def actualizar_c(self):
        if len(self.celsius.text())!=0:
            valido=True
            try:
                self.celsius.setText(str(float(self.celsius.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("Temperatura", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("Temperatura", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.celsius.setFocus()
                self.celsius.selectAll()
                valido=False
            if valido:
                self.dato=Temperature(float(self.celsius.text())).unit("C")
                self.actualizar()
            
    def actualizar_f(self):
        if len(self.fahrenheit.text())!=0:
            valido=True
            try:
                self.fahrenheit.setText(str(float(self.fahrenheit.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("Temperatura", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("Temperatura", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.fahrenheit.setFocus()
                self.fahrenheit.selectAll()
                valido=False
            if valido:
                self.dato=Temperature(float(self.fahrenheit.text())).unit("F")
                self.actualizar()
        
    def actualizar_r(self):
        if len(self.rankine.text())!=0:
            valido=True
            try:
                self.rankine.setText(str(float(self.rankine.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("Temperatura", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("Temperatura", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.rankine.setFocus()
                self.rankine.selectAll()
                valido=False
            if valido:
                self.dato=Temperature(float(self.rankine.text())).unit("R")
                self.actualizar()
            
    def actualizar_re(self):
        if len(self.reaumur.text())!=0:
            valido=True
            try:
                self.reaumur.setText(str(float(self.reaumur.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,QtGui.QApplication.translate("Temperatura", "Error de entrada", None, QtGui.QApplication.UnicodeUTF8), QtGui.QApplication.translate("Temperatura", "Valor no numérico", None, QtGui.QApplication.UnicodeUTF8))
                self.reaumur.setFocus()
                self.reaumur.selectAll()
                valido=False
            if valido:
                self.dato=Temperature(float(self.reaumur.text())).unit("Re")
                self.actualizar()
    
    def actualizar(self):
        if self.semaforo.available()>0:
            self.semaforo.acquire(1)
            self.kelvin.setText(representacion(self.dato))
            self.celsius.setText(representacion(self.dato.C))
            self.fahrenheit.setText(representacion(self.dato.F))
            self.rankine.setText(representacion(self.dato.R))
            self.reaumur.setText(representacion(self.dato.Re))
            self.semaforo.release(1)
    
if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    Temperatura = Ui_temperatura()
    Temperatura.setupUi(Temperatura)
    Temperatura.show()
    sys.exit(app.exec_())
