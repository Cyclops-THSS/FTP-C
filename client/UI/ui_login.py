# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_login.ui'
#
# Created by: PyQt5 UI code generator 5.7
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dlg_login(object):
    def setupUi(self, Dlg_login):
        Dlg_login.setObjectName("Dlg_login")
        Dlg_login.resize(362, 126)
        self.horizontalLayout = QtWidgets.QHBoxLayout(Dlg_login)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        self.label = QtWidgets.QLabel(Dlg_login)
        self.label.setObjectName("label")
        self.verticalLayout.addWidget(self.label)
        self.line_user = QtWidgets.QLineEdit(Dlg_login)
        self.line_user.setObjectName("line_user")
        self.verticalLayout.addWidget(self.line_user)
        self.label_2 = QtWidgets.QLabel(Dlg_login)
        self.label_2.setObjectName("label_2")
        self.verticalLayout.addWidget(self.label_2)
        self.line_pass = QtWidgets.QLineEdit(Dlg_login)
        self.line_pass.setEchoMode(QtWidgets.QLineEdit.PasswordEchoOnEdit)
        self.line_pass.setObjectName("line_pass")
        self.verticalLayout.addWidget(self.line_pass)
        self.horizontalLayout.addLayout(self.verticalLayout)
        self.buttonBox = QtWidgets.QDialogButtonBox(Dlg_login)
        self.buttonBox.setOrientation(QtCore.Qt.Vertical)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.horizontalLayout.addWidget(self.buttonBox)

        self.retranslateUi(Dlg_login)
        self.buttonBox.accepted.connect(Dlg_login.accept)
        self.buttonBox.rejected.connect(Dlg_login.reject)
        QtCore.QMetaObject.connectSlotsByName(Dlg_login)

    def retranslateUi(self, Dlg_login):
        _translate = QtCore.QCoreApplication.translate
        Dlg_login.setWindowTitle(_translate("Dlg_login", "Login"))
        self.label.setText(_translate("Dlg_login", "Username:"))
        self.line_user.setText(_translate("Dlg_login", "anonymous"))
        self.label_2.setText(_translate("Dlg_login", "Password:"))
        self.line_pass.setText(_translate("Dlg_login", "anonymous@"))

