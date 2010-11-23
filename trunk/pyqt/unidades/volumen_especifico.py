#!/usr/bin/python
# -*- coding: utf-8 -*-


from PyQt4 import QtCore, QtGui

from unidades import SpecificVolume
from config import representacion


class Ui_volumen_especifico(QtGui.QDialog):
    def setupUi(self, volumen_especifico):
        
        volumen_especifico.resize(volumen_especifico.minimumSize())
        self.semaforo=QtCore.QSemaphore(1)
        self.gridLayout = QtGui.QGridLayout(volumen_especifico)
        self.label_8 = QtGui.QLabel(volumen_especifico)
        self.label_8.setAlignment(QtCore.Qt.AlignRight)
        self.label_8.setText("l/gr")
        self.gridLayout.addWidget(self.label_8,1,1,1,1)
        self.label_10 = QtGui.QLabel(volumen_especifico)
        self.label_10.setAlignment(QtCore.Qt.AlignRight)
        self.label_10.setText("ft³/lb")
        self.gridLayout.addWidget(self.label_10,2,1,1,1)
        self.label_11 = QtGui.QLabel(volumen_especifico)
        self.label_11.setAlignment(QtCore.Qt.AlignRight)
        self.label_11.setText("gal(US)/lb")
        self.gridLayout.addWidget(self.label_11,3,1,1,1)
        self.label_7 = QtGui.QLabel(volumen_especifico)
        self.label_7.setAlignment(QtCore.Qt.AlignRight)
        self.label_7.setText("l/kg")
        self.gridLayout.addWidget(self.label_7,4,1,1,1)
        self.label_13 = QtGui.QLabel(volumen_especifico)
        self.label_13.setAlignment(QtCore.Qt.AlignRight)
        self.label_13.setText("ft³/Ton(US)")
        self.gridLayout.addWidget(self.label_13,5,1,1,1)
        self.label_9 = QtGui.QLabel(volumen_especifico)
        self.label_9.setAlignment(QtCore.Qt.AlignRight)
        self.label_9.setText("cm³/gr")
        self.gridLayout.addWidget(self.label_9,6,1,1,1)
        self.label_6 = QtGui.QLabel(volumen_especifico)
        self.label_6.setAlignment(QtCore.Qt.AlignRight)
        self.label_6.setText("m³/kg")
        self.gridLayout.addWidget(self.label_6,7,1,1,1)
        self.label_12 = QtGui.QLabel(volumen_especifico)
        self.label_12.setAlignment(QtCore.Qt.AlignRight)
        self.label_12.setText("gal(UK)/lb")
        self.gridLayout.addWidget(self.label_12,8,1,1,1)
        self.label_14 = QtGui.QLabel(volumen_especifico)
        self.label_14.setAlignment(QtCore.Qt.AlignRight)
        self.label_14.setText("ft³/Ton(UK)")
        self.gridLayout.addWidget(self.label_14,9,1,1,1)
        
        self.m3kg = QtGui.QLineEdit(volumen_especifico)
        self.m3kg.setAlignment(QtCore.Qt.AlignRight)
        self.m3kg.setValidator(QtGui.QDoubleValidator(volumen_especifico))
        self.gridLayout.addWidget(self.m3kg,1,2,1,1)
        self.lkg = QtGui.QLineEdit(volumen_especifico)
        self.lkg.setAlignment(QtCore.Qt.AlignRight)
        self.lkg.setValidator(QtGui.QDoubleValidator(volumen_especifico))
        self.gridLayout.addWidget(self.lkg,2,2,1,1)
        self.lg = QtGui.QLineEdit(volumen_especifico)
        self.lg.setAlignment(QtCore.Qt.AlignRight)
        self.lg.setValidator(QtGui.QDoubleValidator(volumen_especifico))
        self.gridLayout.addWidget(self.lg,3,2,1,1)
        self.ccg = QtGui.QLineEdit(volumen_especifico)
        self.ccg.setAlignment(QtCore.Qt.AlignRight)
        self.ccg.setValidator(QtGui.QDoubleValidator(volumen_especifico))
        self.gridLayout.addWidget(self.ccg,4,2,1,1)
        self.ft3lb = QtGui.QLineEdit(volumen_especifico)
        self.ft3lb.setAlignment(QtCore.Qt.AlignRight)
        self.ft3lb.setValidator(QtGui.QDoubleValidator(volumen_especifico))
        self.gridLayout.addWidget(self.ft3lb,5,2,1,1)
        self.galUSlb = QtGui.QLineEdit(volumen_especifico)
        self.galUSlb.setAlignment(QtCore.Qt.AlignRight)
        self.galUSlb.setValidator(QtGui.QDoubleValidator(volumen_especifico))
        self.gridLayout.addWidget(self.galUSlb,6,2,1,1)
        self.galUKlb = QtGui.QLineEdit(volumen_especifico)
        self.galUKlb.setAlignment(QtCore.Qt.AlignRight)
        self.galUKlb.setValidator(QtGui.QDoubleValidator(volumen_especifico))
        self.gridLayout.addWidget(self.galUKlb,7,2,1,1)
        self.ft3tonUS = QtGui.QLineEdit(volumen_especifico)
        self.ft3tonUS.setAlignment(QtCore.Qt.AlignRight)
        self.ft3tonUS.setValidator(QtGui.QDoubleValidator(volumen_especifico))
        self.gridLayout.addWidget(self.ft3tonUS,8,2,1,1)
        self.ft3tonUK = QtGui.QLineEdit(volumen_especifico)
        self.ft3tonUK.setAlignment(QtCore.Qt.AlignRight)
        self.ft3tonUK.setValidator(QtGui.QDoubleValidator(volumen_especifico))
        self.gridLayout.addWidget(self.ft3tonUK,9,2,1,1)
        
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,0,10,1)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,0,1,4)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),10,0,1,4)
        self.gridLayout.addItem(QtGui.QSpacerItem(20,20,QtGui.QSizePolicy.Expanding,QtGui.QSizePolicy.Expanding),0,3,11,1)
        
        self.buttonBox = QtGui.QDialogButtonBox(volumen_especifico)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setCenterButtons(True)        
        self.gridLayout.addWidget(self.buttonBox,11,1,1,2)


        self.retranslateUi(volumen_especifico)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("accepted()"),volumen_especifico.accept)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("rejected()"),volumen_especifico.reject)
        QtCore.QObject.connect(self.m3kg,QtCore.SIGNAL("editingFinished()"),self.actualizar_m3kg)
        QtCore.QObject.connect(self.lkg,QtCore.SIGNAL("editingFinished()"),self.actualizar_lkg)
        QtCore.QObject.connect(self.lg,QtCore.SIGNAL("editingFinished()"),self.actualizar_lg)
        QtCore.QObject.connect(self.ccg,QtCore.SIGNAL("editingFinished()"),self.actualizar_grcm3)
        QtCore.QObject.connect(self.ft3lb,QtCore.SIGNAL("editingFinished()"),self.actualizar_ft3lb)
        QtCore.QObject.connect(self.galUSlb,QtCore.SIGNAL("editingFinished()"),self.actualizar_galUS)
        QtCore.QObject.connect(self.galUKlb,QtCore.SIGNAL("editingFinished()"),self.actualizar_galUK)
        QtCore.QObject.connect(self.ft3tonUS,QtCore.SIGNAL("editingFinished()"),self.actualizar_tonUS)
        QtCore.QObject.connect(self.ft3tonUK,QtCore.SIGNAL("editingFinished()"),self.actualizar_tonUK)
        QtCore.QMetaObject.connectSlotsByName(volumen_especifico)


    def retranslateUi(self, volumen_especifico):
        volumen_especifico.setWindowTitle(QtGui.QApplication.translate("volumen_especifico", "Volumen_especifico", None, QtGui.QApplication.UnicodeUTF8))
        self.label_8.setToolTip(QtGui.QApplication.translate("volumen_especifico", "litros por gramo", None, QtGui.QApplication.UnicodeUTF8))
        self.label_10.setToolTip(QtGui.QApplication.translate("volumen_especifico", "pie cúbico por libra", None, QtGui.QApplication.UnicodeUTF8))
        self.label_11.setToolTip(QtGui.QApplication.translate("volumen_especifico", "galón estadounidense por libra", None, QtGui.QApplication.UnicodeUTF8))
        self.label_11.setWhatsThis(QtGui.QApplication.translate("volumen_especifico", "El galón usado en Estados Unidos o internacional equivale a 231 pulgadas cúbicas o 3,7854 litros", None, QtGui.QApplication.UnicodeUTF8))
        self.label_7.setToolTip(QtGui.QApplication.translate("volumen_especifico", "litro por kilogramo", None, QtGui.QApplication.UnicodeUTF8))
        self.galUKlb.setToolTip(QtGui.QApplication.translate("volumen_especifico", "galón (reino unido) por libra", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setToolTip(QtGui.QApplication.translate("volumen_especifico", "pies cúbicos por tonelada americana", None, QtGui.QApplication.UnicodeUTF8))
        self.label_13.setWhatsThis(QtGui.QApplication.translate("volumen_especifico", "Tonelada corta: equivale a 907,18474 kg, y en inglés se llama 'short ton' aunque en el uso habitual de Estados Unidos se le llama tonelada (ton), como la métrica, y hasta se abrevia t", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setToolTip(QtGui.QApplication.translate("volumen_especifico", "centímetro cúbico por gramo", None, QtGui.QApplication.UnicodeUTF8))
        self.label_6.setToolTip(QtGui.QApplication.translate("volumen_especifico", "metros cúbicos por kilogramo", None, QtGui.QApplication.UnicodeUTF8))
        self.label_12.setToolTip(QtGui.QApplication.translate("volumen_especifico", "galones británicos por libra", None, QtGui.QApplication.UnicodeUTF8))
        self.label_12.setWhatsThis(QtGui.QApplication.translate("volumen_especifico", "El galón usado en el Reino Unido o imperial equivale a 277,41945 pulgadas cúbicas o 4,5461 litros", None, QtGui.QApplication.UnicodeUTF8))
        self.label_14.setToolTip(QtGui.QApplication.translate("volumen_especifico", "pies cúbicos por tonelada británica", None, QtGui.QApplication.UnicodeUTF8))
        self.label_14.setWhatsThis(QtGui.QApplication.translate("volumen_especifico", "La tonelada larga equivale a 1.016,0469088 kg, y en inglés se llama long ton o weight ton", None, QtGui.QApplication.UnicodeUTF8))


    def actualizar_m3kg(self):
        valido=True
        if len(self.m3kg.text())!=0:
            try:
                self.m3kg.setText(str(float(self.m3kg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.m3kg.setFocus()
                self.m3kg.selectAll()
                valido=False
            if valido:
                self.dato=SpecificVolume(float(self.m3kg.text())).unit("m3kg")
                self.actualizar()
    
    def actualizar_lkg(self):
        valido=True
        if len(self.lkg.text())!=0:
            try:
                self.lkg.setText(str(float(self.lkg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.lkg.setFocus()
                self.lkg.selectAll()
                valido=False
            if valido:
                self.dato=SpecificVolume(float(self.lkg.text())).unit("lkg")
                self.actualizar()

    def actualizar_lg(self):
        valido=True
        if len(self.lg.text())!=0:
            try:
                self.lg.setText(str(float(self.lg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.lg.setFocus()
                self.lg.selectAll()
                valido=False
            if valido:
                self.dato=SpecificVolume(float(self.lg.text())).unit("lg")
                self.actualizar()
        
    def actualizar_grcm3(self):
        valido=True
        if len(self.ccg.text())!=0:
            try:
                self.ccg.setText(str(float(self.ccg.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.ccg.setFocus()
                self.ccg.selectAll()
                valido=False
            if valido:
                self.dato=SpecificVolume(float(self.ccg.text())).unit("ccg")
                self.actualizar()
        
    def actualizar_ft3lb(self):
        valido=True
        if len(self.ft3lb.text())!=0:
            try:
                self.ft3lb.setText(str(float(self.ft3lb.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.ft3lb.setFocus()
                self.ft3lb.selectAll()
                valido=False
            if valido:
                self.dato=SpecificVolume(float(self.ft3lb.text())).unit("ft3lb")
                self.actualizar()
        
    def actualizar_galUS(self):
        valido=True
        if len(self.galUSlb.text())!=0:
            try:
                self.galUSlb.setText(str(float(self.galUSlb.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.galUSlb.setFocus()
                self.galUSlb.selectAll()
                valido=False
            if valido:
                self.dato=SpecificVolume(float(self.galUSlb.text())).unit("galUSlb")
                self.actualizar()
        
    def actualizar_galUK(self):
        valido=True
        if len(self.galUKlb.text())!=0:
            try:
                self.galUKlb.setText(str(float(self.galUKlb.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.galUKlb.setFocus()
                self.galUKlb.selectAll()
                valido=False
            if valido:
                self.dato=SpecificVolume(float(self.galUKlb.text())).unit("galUKlb")
                self.actualizar()

    def actualizar_tonUS(self):
        valido=True
        if len(self.ft3tonUS.text())!=0:
            try:
                self.ft3tonUS.setText(str(float(self.ft3tonUS.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.ft3tonUS.setFocus()
                self.ft3tonUS.selectAll()
                valido=False
            if valido:
                self.dato=SpecificVolume(float(self.ft3tonUS.text())).unit("ft3tonUS")
                self.actualizar()
        
    def actualizar_tonUK(self):
        valido=True
        if len(self.ft3tonUK.text())!=0:
            try:
                self.ft3tonUK.setText(str(float(self.ft3tonUK.text())))
            except ValueError:
                QtGui.QMessageBox.warning(self,"Error de entrada", u"Valor no numérico")
                self.ft3tonUK.setFocus()
                self.ft3tonUK.selectAll()
                valido=False
            if valido:
                self.dato=SpecificVolume(float(self.ft3tonUK.text())).unit("ft3tonUK")
                self.actualizar()
      
    def actualizar(self):
        if self.semaforo.available()>0:
            self.semaforo.acquire(1)
            self.m3kg.setText(representacion(self.dato.m3kg))
            self.lg.setText(representacion(self.dato.lg))
            self.lkg.setText(representacion(self.dato.lkg))
            self.ccg.setText(representacion(self.dato.ccg))
            self.ft3lb.setText(representacion(self.dato.ft3lb))
            self.galUSlb.setText(representacion(self.dato.galUSlb))
            self.galUKlb.setText(representacion(self.dato.galUKlb))
            self.ft3tonUS.setText(representacion(self.dato.ft3tonUS))
            self.ft3tonUK.setText(representacion(self.dato.ft3tonUK))
            self.semaforo.release(1)

if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    volumen_especifico = Ui_volumen_especifico()
    volumen_especifico.setupUi(volumen_especifico)
    volumen_especifico.show()
    sys.exit(app.exec_())
