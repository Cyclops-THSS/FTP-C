from PyQt5 import QtCore, QtGui, QtWidgets
import sys
from UI.ui_client import Ui_MainWindow
from UI.ui_connect import Ui_Dlg_connect
from UI.ui_login import Ui_Dlg_login
from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QDialog, QFileDialog, QInputDialog, QMainWindow
from ftplib import FTP
import ntpath

app = QtWidgets.QApplication(sys.argv)


class ConnectDlg(QDialog, Ui_Dlg_connect):

    def __init__(self):
        super(ConnectDlg, self).__init__()
        self.setupUi(self)


class LoginDlg(QDialog, Ui_Dlg_login):

    def __init__(self):
        super(LoginDlg, self).__init__()
        self.setupUi(self)


class Main(QMainWindow, Ui_MainWindow):

    dlg_connect = ConnectDlg()
    dlg_login = LoginDlg()
    ftp = FTP()

    def __init__(self):
        super(Main, self).__init__()
        self.setupUi(self)
        self.btn_connect.clicked.connect(self.s_connect)
        self.btn_quit.clicked.connect(self.s_quit)
        self.btn_login.clicked.connect(self.s_login)
        self.btn_syst.clicked.connect(self.s_syst)
        self.btn_type.clicked.connect(self.s_type)
        self.rb_port.clicked.connect(self.s_port)
        self.rb_pasv.clicked.connect(self.s_pasv)
        self.btn_upload.clicked.connect(self.s_upload)
        self.btn_download.clicked.connect(self.s_download)
        self.btn_send.clicked.connect(self.s_send)

    def set_enabled(self, bool, *args):
        for x in args:
            getattr(self, x).setEnabled(bool)

    def s_connect(self):
        if self.dlg_connect.exec_():
            try:
                self.textBrowser.append(self.ftp.connect(
                    self.dlg_connect.line_ip.text(), int(self.dlg_connect.line_port.text())))
                self.set_enabled(True, 'btn_login', 'btn_syst',
                                 'btn_type', 'btn_quit', 'lineEdit', 'btn_send')
            except Exception as e:
                self.textBrowser.append(str(e))

    def s_quit(self):
        try:
            self.textBrowser.append(self.ftp.quit())
            self.set_enabled(False, 'btn_login', 'btn_syst', 'btn_type', 'btn_quit',
                             'lineEdit', 'btn_send', 'rb_port', 'rb_pasv', 'btn_upload', 'btn_download')
        except Exception as e:
            self.textBrowser.append(str(e))

    def s_login(self):
        if self.dlg_login.exec_():
            try:
                self.textBrowser.append(self.ftp.login(
                    self.dlg_login.line_user.text(), self.dlg_login.line_pass.text()))
                self.set_enabled(True, 'rb_port', 'rb_pasv',
                                 'btn_upload', 'btn_download')
            except Exception as e:
                self.textBrowser.append(str(e))

    def s_syst(self):
        try:
            self.textBrowser.append(self.ftp.sendcmd('SYST'))
        except Exception as e:
            self.textBrowser.append(str(e))

    def s_type(self):
        text, ok = QInputDialog.getText(self, 'Type', 'New type:')
        if ok:
            try:
                self.textBrowser.append(self.ftp.sendcmd('TYPE ' + text))
            except Exception as e:
                self.textBrowser.append(str(e))

    def s_port(self):
        try:
            self.ftp.set_pasv(False)
        except Exception as e:
            self.textBrowser.append(str(e))

    def s_pasv(self):
        try:
            self.ftp.set_pasv(True)
        except Exception as e:
            self.textBrowser.append(str(e))

    def s_upload(self):
        file, _ = QFileDialog.getOpenFileName(self)
        if file:
            fn = ntpath.basename(file)
            try:
                self.textBrowser.append(self.ftp.storbinary(
                    'STOR %s' % fn, open(file, 'rb')))
            except Exception as e:
                self.textBrowser.append(str(e))

    def s_download(self):
        text, ok = QInputDialog.getText(
            self, 'Download File', 'File path on server:')
        if ok:
            try:
                file, _ = QFileDialog.getSaveFileName(self)
                if file:
                    self.textBrowser.append(self.ftp.retrbinary(
                        'RETR %s' % text, open(file, 'wb').write))
            except Exception as e:
                self.textBrowser.append(str(e))

    def s_send(self):
        text = self.lineEdit.text()
        if text != '':
            self.textBrowser.append(self.ftp.sendcmd(text))

if __name__ == '__main__':
    main = Main()
    main.show()
    sys.exit(app.exec_())
