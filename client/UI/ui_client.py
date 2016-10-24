# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_client.ui'
#
# Created by: PyQt5 UI code generator 5.7
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(596, 425)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(self.centralwidget)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.group_btn = QtWidgets.QGroupBox(self.centralwidget)
        self.group_btn.setObjectName("group_btn")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.group_btn)
        self.verticalLayout.setObjectName("verticalLayout")
        self.btn_connect = QtWidgets.QPushButton(self.group_btn)
        self.btn_connect.setObjectName("btn_connect")
        self.verticalLayout.addWidget(self.btn_connect)
        self.btn_login = QtWidgets.QPushButton(self.group_btn)
        self.btn_login.setEnabled(False)
        self.btn_login.setObjectName("btn_login")
        self.verticalLayout.addWidget(self.btn_login)
        self.btn_syst = QtWidgets.QPushButton(self.group_btn)
        self.btn_syst.setEnabled(False)
        self.btn_syst.setObjectName("btn_syst")
        self.verticalLayout.addWidget(self.btn_syst)
        self.btn_type = QtWidgets.QPushButton(self.group_btn)
        self.btn_type.setEnabled(False)
        self.btn_type.setObjectName("btn_type")
        self.verticalLayout.addWidget(self.btn_type)
        self.rb_port = QtWidgets.QRadioButton(self.group_btn)
        self.rb_port.setEnabled(False)
        self.rb_port.setObjectName("rb_port")
        self.verticalLayout.addWidget(self.rb_port)
        self.rb_pasv = QtWidgets.QRadioButton(self.group_btn)
        self.rb_pasv.setEnabled(False)
        self.rb_pasv.setChecked(True)
        self.rb_pasv.setObjectName("rb_pasv")
        self.verticalLayout.addWidget(self.rb_pasv)
        self.btn_upload = QtWidgets.QPushButton(self.group_btn)
        self.btn_upload.setEnabled(False)
        self.btn_upload.setObjectName("btn_upload")
        self.verticalLayout.addWidget(self.btn_upload)
        self.btn_download = QtWidgets.QPushButton(self.group_btn)
        self.btn_download.setEnabled(False)
        self.btn_download.setObjectName("btn_download")
        self.verticalLayout.addWidget(self.btn_download)
        self.btn_quit = QtWidgets.QPushButton(self.group_btn)
        self.btn_quit.setEnabled(False)
        self.btn_quit.setObjectName("btn_quit")
        self.verticalLayout.addWidget(self.btn_quit)
        self.horizontalLayout_2.addWidget(self.group_btn)
        self.group_cmd = QtWidgets.QGroupBox(self.centralwidget)
        self.group_cmd.setObjectName("group_cmd")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout(self.group_cmd)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.lineEdit = QtWidgets.QLineEdit(self.group_cmd)
        self.lineEdit.setEnabled(False)
        self.lineEdit.setObjectName("lineEdit")
        self.horizontalLayout.addWidget(self.lineEdit)
        self.btn_send = QtWidgets.QPushButton(self.group_cmd)
        self.btn_send.setEnabled(False)
        self.btn_send.setObjectName("btn_send")
        self.horizontalLayout.addWidget(self.btn_send)
        self.verticalLayout_2.addLayout(self.horizontalLayout)
        self.textBrowser = QtWidgets.QTextBrowser(self.group_cmd)
        self.textBrowser.setObjectName("textBrowser")
        self.verticalLayout_2.addWidget(self.textBrowser)
        self.horizontalLayout_2.addWidget(self.group_cmd)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 596, 22))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "FTP Client"))
        self.group_btn.setTitle(_translate("MainWindow", "Tool Button"))
        self.btn_connect.setText(_translate("MainWindow", "Connect"))
        self.btn_login.setText(_translate("MainWindow", "Login"))
        self.btn_syst.setText(_translate("MainWindow", "SYST"))
        self.btn_type.setText(_translate("MainWindow", "TYPE"))
        self.rb_port.setText(_translate("MainWindow", "Active"))
        self.rb_pasv.setText(_translate("MainWindow", "Passive"))
        self.btn_upload.setText(_translate("MainWindow", "Upload"))
        self.btn_download.setText(_translate("MainWindow", "Download"))
        self.btn_quit.setText(_translate("MainWindow", "QUIT"))
        self.group_cmd.setTitle(_translate("MainWindow", "Command Line Tool"))
        self.btn_send.setText(_translate("MainWindow", "Send"))
        self.textBrowser.setHtml(_translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'.SF NS Text\'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#0000ff;\">Welcome to FTP Client!</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#0000ff;\">Click Connect to begin~</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#0000ff;\"><br /></p></body></html>"))

