# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_connect.ui'
#
# Created by: PyQt5 UI code generator 5.7
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dlg_connect(object):
    def setupUi(self, Dlg_connect):
        Dlg_connect.setObjectName("Dlg_connect")
        Dlg_connect.resize(381, 78)
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(Dlg_connect)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        self.label = QtWidgets.QLabel(Dlg_connect)
        self.label.setObjectName("label")
        self.verticalLayout.addWidget(self.label)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.line_ip = QtWidgets.QLineEdit(Dlg_connect)
        self.line_ip.setObjectName("line_ip")
        self.horizontalLayout.addWidget(self.line_ip)
        self.label_2 = QtWidgets.QLabel(Dlg_connect)
        self.label_2.setObjectName("label_2")
        self.horizontalLayout.addWidget(self.label_2)
        self.line_port = QtWidgets.QLineEdit(Dlg_connect)
        self.line_port.setObjectName("line_port")
        self.horizontalLayout.addWidget(self.line_port)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.horizontalLayout_2.addLayout(self.verticalLayout)
        self.buttonBox = QtWidgets.QDialogButtonBox(Dlg_connect)
        self.buttonBox.setOrientation(QtCore.Qt.Vertical)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.horizontalLayout_2.addWidget(self.buttonBox)

        self.retranslateUi(Dlg_connect)
        self.buttonBox.accepted.connect(Dlg_connect.accept)
        self.buttonBox.rejected.connect(Dlg_connect.reject)
        QtCore.QMetaObject.connectSlotsByName(Dlg_connect)

    def retranslateUi(self, Dlg_connect):
        _translate = QtCore.QCoreApplication.translate
        Dlg_connect.setWindowTitle(_translate("Dlg_connect", "Connect"))
        self.label.setText(_translate("Dlg_connect", "Server IP and Port:"))
        self.line_ip.setInputMask(_translate("Dlg_connect", "000.000.000.000;_"))
        self.line_ip.setText(_translate("Dlg_connect", "127.0.0.1"))
        self.label_2.setText(_translate("Dlg_connect", ":"))
        self.line_port.setInputMask(_translate("Dlg_connect", "90000"))
        self.line_port.setText(_translate("Dlg_connect", "21"))

