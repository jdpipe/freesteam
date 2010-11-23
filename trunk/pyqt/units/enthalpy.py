#!/usr/bin/python
# -*- coding: utf-8 -*-


from PyQt4 import QtCore, QtGui

from unidades import Enthalpy
from config import representacion


class Ui_entalpia(QtGui.QDialog):
    def setupUi(self, entalpia):
        entalpia.resize(350, entalpia.minimumHeight())
        self.semaforo=QtCore.QSemaphore(1)
        self.gridLayout = QtGui.QGridLayout(entalpia)
        

        self.label = QtGui.QLabel(entalpia)
        self.label.setText("J/kg")
        self.label.setAlignment(QtCore.Qt.AlignRight)
        self.gridLayout.addWidget(self.label,1,1,1,1)
        self.label_6 = QtGui.QLabel(entalpia)
        self.label_6.setAlignment(QtCore.Qt.AlignRight)
        self.label_6.setText("kJ/kg")
        self.gridLayout.addWidget(self.label_6,2,1,1,1)
        self.label_2 = QtGui.QLabel(entalpia)
        self.label_2.setAlignment(QtCore.Qt.AlignRight)
        self.label_2.setText("MJ/kg")
        self.gridLayout.addWidget(self.label_2,3,1,1,1)
        self.label_3 = QtGui.QLabel(entalpia)
        self.label_3.setAlignment(QtCore.Qt.AlignRight)
        self.label_3.setText("kWh/kg")
        self.gridLayout.addWidget(self.label_3,4,1,1,1)
        self.label_4 = QtGui.QLabel(entalpia)
        self.label_4.setAlignment(QtCore.Qt.AlignRight)
        self.label_4.setText("Btu/lb")
        self.gridLayout.addWidget(self.label_4,5,1,1,1)
        self.label_12 = QtGui.QLabel(entalpia)
        self.label_12.setAlignment(QtCore.Qt.AlignRight)
        self.label_12.setText("cal/kg")
        self.gridLayout.addWidget(self.label_12,1,4,1,1)
        self.label_8 = QtGui.QLabel(entalpia)
        self.label_8.setAlignment(QtCore.Qt.AlignRight)
        self.label_8.setText("kcal/kg")
        self.gridLayout.addWidget(self.label_8,2,4,1,1)
        self.label_9 = QtGui.QLabel(entalpia)
        self.label_9.setAlignment(QtCore.Qt.AlignRight)
        self.label_9.setText("kcal/g")
        self.gridLayout.addWidget(self.label_9,3,4,1,1)
        self.label_16 = QtGui.QLabel(entalpia)
        self.label_16.setAlignment(QtCore.Qt.AlignRight)
        self.label_16.setText("cal/g")
        self.gridLayout.addWidget(self.label_16,4,4,1,1)
        self.label_11 = QtGui.QLabel(entalpia)
        self.label_11.setAlignment(QtCore.Qt.AlignRight)
        self.label_11.setText("cal/lb")
        self.gridLayout.addWidget(self.label_11,5,4,1,1)

        
        self.Jkg = QtGui.QLineEdit(entalpia)
        self.Jkg.setAlignment(QtCore.Qt.AlignRight)
        self.Jkg.setValidator(QtGui.QDoubleValidator(entalpia))
        self.gridLayout.addWidget(self.Jkg,1,2,1,1)
        self.kJkg = QtGui.QLineEdit(entalpia)
        self.kJkg.setAlignment(QtCore.Qt.AlignRight)
        self.kJkg.setValidator(QtGui.QDoubleValidator(entalpia))
        self.gridLayout.addWidget(self.kJkg,2,2,1,1)
        self.MJkg = QtGui.QLineEdit(entalpia)
        self.MJkg.setAlignment(QtCore.Qt.AlignRight)
        self.MJkg.setValidator(QtGui.QDoubleValidator(entalpia))
        self.gridLayout.addWidget(self.MJkg,3,2,1,1)
        self.kWhkg = QtGui.QLineEdit(entalpia)
        self.kWhkg.setAlignment(QtCore.Qt.AlignRight)
        self.kWhkg.setValidator(QtGui.QDoubleValidator(entalpia))
        self.gridLayout.addWidget(self.kWhkg,4,2,1,1)
        self.Btulb = QtGui.QLineEdit(entalpia)
        self.Btulb.setAlignment(QtCore.Qt.AlignRight)
        self.Btulb.setValidator(QtGui.QDoubleValidator(entalpia))
        self.gridLayout.addWidget(self.Btulb,5,2,1,1)
        self.calkg = QtGui.QLineEdit(entalpia)
        self.calkg.setAlignment(QtCore.Qt.AlignRight)
        self.calkg.setValidator(QtGui.QDoubleValidator(entalpia))
        self.gridLayout.addWidget(self.calkg,1,5,1,1)
        self.kcalkg = QtGui.QLineEdit(entalpia)
        self.kcalkg.setAlignment(QtCore.Qt.AlignRight)
        self.kcalkg.setValidator(QtGui.QDoubleValidator(entalpia))
        self.gridLayout.addWidget(self.kcalkg,2,5,1,1)
        self.kcalg = QtGui.QLineEdit(entalpia)
        self.kcalg.setAlignment(QtCore.Qt.AlignRight)
        self.kcalg.setValidator(QtGui.QDoubleValidator(entalpia))
        self.gridLayout.addWidget(self.kcalg,3,5,1,1)
        self.calg = QtGui.QLineEdit(entalpia)
        self.calg.setAlignment(QtCore.Qt.AlignRight)
        self.calg.setValidator(QtGui.QDoubleValidator(entalpia))
        self.gridLayout.addWidget(self.calg,4,5,1,1)
        self.callb = QtGui.QLineEdit(entalpia)
        self.callb.setAlignment(QtCore.Qt.AlignRight)
        self.callb.setValidator(QtGui.QDoubleValidator(entalpia))
        self.gridLayout.addWidget(self.callb,5,5,1,1)
        
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,0,6,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,0,1,4)
        self.gridLayout.addItem(QtGui.QSpacerItem(10,10,QtGui.QSizePolicy.Fixed,QtGui.QSizePolicy.Fixed),0,3,6,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,6,6,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),6,0,1,4) 
 
        self.buttonBox = QtGui.QDialogButtonBox(entalpia)
        self.buttonBox.setGeometry(QtCore.QRect(0,190,280,32))
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setCenterButtons(True)        
        self.gridLayout.addWidget(self.buttonBox,7,1,1,5)

        self.retranslateUi(entalpia)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("accepted()"),entalpia.accept)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("rejected()"),entalpia.reject)
        QtCore.QObject.connect(self.Jkg,QtCore.SIGNAL("editingFinished()"),self.actualizar_Jkg)
        QtCore.QObject.connect(self.kJkg,QtCore.SIGNAL("editingFinished()"),self.actualizar_kJkg)
        QtCore.QObject.connect(self.MJkg,QtCore.SIGNAL("editingFinished()"),self.actualizar_MJkg)
        QtCore.QObject.connect(self.kWhkg,QtCore.SIGNAL("editingFinished()"),self.actualizar_kWhkg)
        QtCore.QObject.connect(self.Btulb,QtCore.SIGNAL("editingFinished()"),self.actualizar_Btulb)
        QtCore.QObject.connect(self.calkg,QtCore.SIGNAL("editingFinished()"),self.actualizar_calkg)        
        QtCore.QObject.connect(self.kcalkg,QtCore.SIGNAL("editingFinished()"),self.actualizar_kcalkg)        
        QtCore.QObject.connect(self.kcalg,QtCore.SIGNAL("editingFinished()"),self.actualizar_kcalg)        
        QtCore.QObject.connect(self.calg,QtCore.SIGNAL("editingFinished()"),self.actualizar_calg)        
        QtCore.QObject.connect(self.callb,QtCore.SIGNAL("editingFinished()"),self.actualizar_callb)        
        QtCore.QMetaObject.connectSlotsByName(entalpia)


    def retranslateUi(self, entalpia):
        entalpia.setWindowTitle(QtGui.QApplication.translate("entalpia", "Entalpia", None, QtGui.QApplication.UnicodeUTF8))


    def actualizar_Jkg(self):
        valido=True
        if len(self.Jkg.text())!=0:
            try:
                self.Jkg.setText(str(float(self.Jkg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.Jkg.setFocus()
                self.Jkg.selectAll()
                valido=False
            if valido:
                self.dato=Enthalpy(float(self.Jkg.text()))
                self.actualizar()
                
    def actualizar_kJkg(self):
        valido=True
        if len(self.kJkg.text())!=0:
            try:
                self.kJkg.setText(str(float(self.kJkg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.kJkg.setFocus()
                self.kJkg.selectAll()
                valido=False
            if valido:
                self.dato=Enthalpy(float(self.kJkg.text())).unit("kJkg")
                self.actualizar()           
           
    def actualizar_MJkg(self):
        valido=True
        if len(self.MJkg.text())!=0:
            try:
                self.MJkg.setText(str(float(self.MJkg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.MJkg.setFocus()
                self.MJkg.selectAll()
                valido=False
            if valido:
                self.dato=Enthalpy(float(self.MJkg.text())).unit("MJkg")
                self.actualizar()

    def actualizar_kWhkg(self):
        valido=True
        if len(self.kWhkg.text())!=0:
            try:
                self.kWhkg.setText(str(float(self.kWhkg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.kWhkg.setFocus()
                self.kWhkg.selectAll()
                valido=False
            if valido:
                self.dato=Enthalpy(float(self.kWhkg.text())).unit("kWhkg")
                self.actualizar()
    
    def actualizar_Btulb(self):
        valido=True
        if len(self.Btulb.text())!=0:
            try:
                self.Btulb.setText(str(float(self.Btulb.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.Btulb.setFocus()
                self.Btulb.selectAll()
                valido=False
            if valido:
                self.dato=Enthalpy(float(self.Btulb.text())).unit("Btulb")
                self.actualizar()

    def actualizar_calkg(self):
        valido=True
        if len(self.calkg.text())!=0:
            try:
                self.calkg.setText(str(float(self.calkg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.calkg.setFocus()
                self.calkg.selectAll()
                valido=False
            if valido:
                self.dato=Enthalpy(float(self.calkg.text())).unit("calkg")
                self.actualizar()
                
    def actualizar_kcalkg(self):
        valido=True
        if len(self.kcalkg.text())!=0:
            try:
                self.kcalkg.setText(str(float(self.kcalkg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.kcalkg.setFocus()
                self.kcalkg.selectAll()
                valido=False
            if valido:
                self.dato=Enthalpy(float(self.kcalkg.text())).unit("kcalkg")
                self.actualizar()
                
    def actualizar_kcalg(self):
        valido=True
        if len(self.kcalg.text())!=0:
            try:
                self.kcalg.setText(str(float(self.kcalg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.kcalg.setFocus()
                self.kcalg.selectAll()
                valido=False
            if valido:
                self.dato=Enthalpy(float(self.kcalg.text())).unit("kcalg")
                self.actualizar()
                
    def actualizar_callb(self):
        valido=True
        if len(self.callb.text())!=0:
            try:
                self.callb.setText(str(float(self.callb.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.callb.setFocus()
                self.callb.selectAll()
                valido=False
            if valido:
                self.dato=Enthalpy(float(self.callb.text())).unit("callb")
                self.actualizar()

    def actualizar_calg(self):
        valido=True
        if len(self.calg.text())!=0:
            try:
                self.calg.setText(str(float(self.calg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.calg.setFocus()
                self.calg.selectAll()
                valido=False
            if valido:
                self.dato=Enthalpy(float(self.calg.text())).unit("calg")
                self.actualizar()

    def actualizar(self):
        if self.semaforo.available()>0:
            self.semaforo.acquire(1)
            self.Jkg.setText(representacion(self.dato.Jkg))
            self.kJkg.setText(representacion(self.dato.kJkg))
            self.MJkg.setText(representacion(self.dato.MJkg))
            self.kWhkg.setText(representacion(self.dato.kWhkg))
            self.Btulb.setText(representacion(self.dato.Btulb))
            self.calkg.setText(representacion(self.dato.calkg))
            self.kcalkg.setText(representacion(self.dato.kcalkg))
            self.calg.setText(representacion(self.dato.calg))
            self.kcalg.setText(representacion(self.dato.kcalg))
            self.callb.setText(representacion(self.dato.callb))
            self.semaforo.release(1)


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    entalpia = Ui_entalpia()
    entalpia.setupUi(entalpia)
    entalpia.show()
    sys.exit(app.exec_())

