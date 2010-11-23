#!/usr/bin/python
# -*- coding: utf-8 -*-


from PyQt4 import QtCore, QtGui

from unidades import SpecificHeat
from config import representacion


class Ui_entropia(QtGui.QDialog):
    def setupUi(self, entropia):
        entropia.resize(entropia.minimumSize())
        self.semaforo=QtCore.QSemaphore(1)
        self.gridLayout = QtGui.QGridLayout(entropia)


        self.label = QtGui.QLabel(entropia)
        self.label.setAlignment(QtCore.Qt.AlignRight)
        self.label.setText("J/kgK")
        self.gridLayout.addWidget(self.label,1,1,1,1)
        self.label_6 = QtGui.QLabel(entropia)
        self.label_6.setAlignment(QtCore.Qt.AlignRight)
        self.label_6.setText("kJ/kgK")
        self.gridLayout.addWidget(self.label_6,2,1,1,1)
        self.label_3 = QtGui.QLabel(entropia)
        self.label_3.setAlignment(QtCore.Qt.AlignRight)
        self.label_3.setText("kWh/kgK")
        self.gridLayout.addWidget(self.label_3,3,1,1,1)
        self.label_4 = QtGui.QLabel(entropia)
        self.label_4.setAlignment(QtCore.Qt.AlignRight)
        self.label_4.setText("Btu/lbF")
        self.gridLayout.addWidget(self.label_4,4,1,1,1)
        self.label_8 = QtGui.QLabel(entropia)
        self.label_8.setAlignment(QtCore.Qt.AlignRight)
        self.label_8.setText("kcal/gK")
        self.gridLayout.addWidget(self.label_8,5,1,1,1)
        self.label_12 = QtGui.QLabel(entropia)
        self.label_12.setAlignment(QtCore.Qt.AlignRight)
        self.label_12.setText("kcal/kgK")
        self.gridLayout.addWidget(self.label_12,6,1,1,1)

        
        self.JkgK = QtGui.QLineEdit(entropia)
        self.JkgK.setAlignment(QtCore.Qt.AlignRight)
        self.JkgK.setValidator(QtGui.QDoubleValidator(entropia))
        self.gridLayout.addWidget(self.JkgK,1,2,1,1)
        self.kJkgK = QtGui.QLineEdit(entropia)
        self.kJkgK.setAlignment(QtCore.Qt.AlignRight)
        self.kJkgK.setValidator(QtGui.QDoubleValidator(entropia))
        self.gridLayout.addWidget(self.kJkgK,2,2,1,1)
        self.kWhkgK = QtGui.QLineEdit(entropia)
        self.kWhkgK.setAlignment(QtCore.Qt.AlignRight)
        self.kWhkgK.setValidator(QtGui.QDoubleValidator(entropia))
        self.gridLayout.addWidget(self.kWhkgK,3,2,1,1)
        self.BtulbF = QtGui.QLineEdit(entropia)
        self.BtulbF.setAlignment(QtCore.Qt.AlignRight)
        self.BtulbF.setValidator(QtGui.QDoubleValidator(entropia))
        self.gridLayout.addWidget(self.BtulbF,4,2,1,1)
        self.kcalgK = QtGui.QLineEdit(entropia)
        self.kcalgK.setAlignment(QtCore.Qt.AlignRight)
        self.kcalgK.setValidator(QtGui.QDoubleValidator(entropia))
        self.gridLayout.addWidget(self.kcalgK,5,2,1,1)
        self.kcalkgK = QtGui.QLineEdit(entropia)
        self.kcalkgK.setAlignment(QtCore.Qt.AlignRight)
        self.kcalkgK.setValidator(QtGui.QDoubleValidator(entropia))
        self.gridLayout.addWidget(self.kcalkgK,6,2,1,1)
        
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,0,6,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,0,1,3)
        self.gridLayout.addItem(QtGui.QSpacerItem(5,5,QtGui.QSizePolicy.Fixed,QtGui.QSizePolicy.Fixed),0,3,6,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,3,6,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),7,0,1,3)
        
        self.buttonBox = QtGui.QDialogButtonBox(entropia)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setCenterButtons(True)        
        self.gridLayout.addWidget(self.buttonBox,8,1,1,3)

        self.retranslateUi(entropia)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("accepted()"),entropia.accept)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("rejected()"),entropia.reject)
        QtCore.QObject.connect(self.JkgK,QtCore.SIGNAL("editingFinished()"),self.actualizar_JkgK)
        QtCore.QObject.connect(self.kJkgK,QtCore.SIGNAL("editingFinished()"),self.actualizar_kJkgK)
        QtCore.QObject.connect(self.kcalkgK,QtCore.SIGNAL("editingFinished()"),self.actualizar_kcalkgK)
        QtCore.QObject.connect(self.kcalgK,QtCore.SIGNAL("editingFinished()"),self.actualizar_kcalgK)
        QtCore.QObject.connect(self.kWhkgK,QtCore.SIGNAL("editingFinished()"),self.actualizar_kWhkgK)
        QtCore.QObject.connect(self.BtulbF,QtCore.SIGNAL("editingFinished()"),self.actualizar_BtulbF)
        QtCore.QMetaObject.connectSlotsByName(entropia)


    def retranslateUi(self, entropia):
        entropia.setWindowTitle(QtGui.QApplication.translate("entropia", "Entropia", None, QtGui.QApplication.UnicodeUTF8))

    def actualizar_kJkgK(self):
        valido=True
        if len(self.kJkgK.text())!=0:
            try:
                self.kJkgK.setText(str(float(self.kJkgK.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.kJkgK.setFocus()
                self.kJkgK.selectAll()
                valido=False
            if valido:
                self.dato=SpecificHeat(float(self.kJkgK.text())).unit("kJkgK")
                self.actualizar()

    def actualizar_JkgK(self):
        valido=True
        if len(self.JkgK.text())!=0:
            try:
                self.JkgK.setText(str(float(self.JkgK.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.JkgK.setFocus()
                self.JkgK.selectAll()
                valido=False
            if valido:
                self.dato=SpecificHeat(float(self.JkgK.text())).unit("JkgK")
                self.actualizar()

    def actualizar_kcalkgK(self):
        valido=True
        if len(self.kcalkgK.text())!=0:
            try:
                self.kcalkgK.setText(str(float(self.kcalkgK.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.kcalkgK.setFocus()
                self.kcalkgK.selectAll()
                valido=False
            if valido:
                self.dato=SpecificHeat(float(self.kcalkgK.text())).unit("kcalkgK")
                self.actualizar()

    def actualizar_kcalgK(self):
        valido=True
        if len(self.kcalgK.text())!=0:
            try:
                self.kcalgK.setText(str(float(self.kcalgK.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.kcalgK.setFocus()
                self.kcalgK.selectAll()
                valido=False
            if valido:
                self.dato=SpecificHeat(float(self.kcalgK.text())).unit("kcalgK")
                self.actualizar()

    def actualizar_kWhkgK(self):
        valido=True
        if len(self.kWhkgK.text())!=0:
            try:
                self.kWhkgK.setText(str(float(self.kWhkgK.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.kWhkgK.setFocus()
                self.kWhkgK.selectAll()
                valido=False
            if valido:
                self.dato=SpecificHeat(float(self.kWhkgK.text())).unit("kWhkgK")
                self.actualizar()

    def actualizar_BtulbF(self):
        valido=True
        if len(self.BtulbF.text())!=0:
            try:
                self.BtulbF.setText(str(float(self.BtulbF.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.BtulbF.setFocus()
                self.BtulbF.selectAll()
                valido=False
            if valido:
                self.dato=SpecificHeat(float(self.BtulbF.text())).unit("BtulbF")
                self.actualizar()

    def actualizar(self):
        if self.semaforo.available()>0:
            self.semaforo.acquire(1)
            self.JkgK.setText(representacion(self.dato.JkgK))
            self.kJkgK.setText(representacion(self.dato.kJkgK))
            self.kcalkgK.setText(representacion(self.dato.kcalkgK))
            self.kcalgK.setText(representacion(self.dato.kcalgK))
            self.BtulbF.setText(representacion(self.dato.BtulbF))
            self.kWhkgK.setText(representacion(self.dato.kWhkgK))
            self.semaforo.release(1)


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    entropia= Ui_entropia()
    entropia.setupUi(entropia)
    entropia.show()
    sys.exit(app.exec_())

