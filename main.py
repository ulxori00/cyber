from helper import *

WIDTH = 720
HEIGHT = 480
MAIN_WINDOW = 0
ENCRYPTION_WINDOW = 1
DECRYPTION_WINDOW = 2
PK_PATH = "PublicKeys/"

class MainWindow(QDialog):

    def __init__(self):
        super(MainWindow, self).__init__()
        loadUi("mainWindow.ui",self)
        self.ecrypt_button.clicked.connect(self.show_encryption_window)
        self.decrypt_button.clicked.connect(self.show_decryption_window)


    def show_encryption_window(self):

        widget.setCurrentIndex(ENCRYPTION_WINDOW)

    def show_decryption_window(self):
        widget.setCurrentIndex(DECRYPTION_WINDOW)


class SubWindow(QDialog):


    def __init__(self):
        super(SubWindow, self).__init__()


    def load(self, path, func_name):
        loadUi(path, self)
        self.func_name = func_name
        self.file_path = ""
        self.key_path = ""
        self.file_button.clicked.connect(self.browse_file)
        if func_name == 'decrypt':
            self.key_button.clicked.connect(self.browse_key)
        else:
            print('hello')
        self.submit_button.clicked.connect(self.submit_button_clicked)
        self.arrow_button.clicked.connect(self.goto_main_window)

    def browse_file(self):
        self.file_path = QFileDialog.getOpenFileName(self, 'Open file')[0]
        self.file_line.setText(self.file_path)

    def browse_key(self):
        self.key_path = QFileDialog.getOpenFileName(self, 'Open file')[0]
        self.key_line.setText(self.key_path)

    def load_public_keys(self, public_keys):
        self.public_keys = public_keys
        for key_owner in public_keys.keys():
            self.comboBox.addItem(key_owner)

    def goto_main_window(self):
        self.error.setText("")
        widget.setCurrentIndex(MAIN_WINDOW)

    def submit_button_clicked(self):

        if self.func_name =='encrypt':
            self.key_path = self.public_keys[self.comboBox.currentText()]

        if len(self.key_path)==0 or len(self.file_path)==0:
            self.error.setText("Please input all fields")
        else:
            eval(self.func_name)(self.key_path, self.file_path)
            self.goto_main_window()





#Main
public_keys = load_public_keys(PK_PATH)

app = QApplication(sys.argv)
main_window = MainWindow()
widget = QtWidgets.QStackedWidget()
widget.addWidget(main_window)
widget.setFixedHeight(HEIGHT)
widget.setFixedWidth(WIDTH)

encryption_window = SubWindow()
encryption_window.load("enryptionWindow.ui", 'encrypt')
encryption_window.load_public_keys(public_keys)

decryption_window = SubWindow()
decryption_window.load("decryptionWindow.ui",'decrypt')


widget.addWidget(encryption_window)
widget.addWidget(decryption_window)
widget.setCurrentIndex(MAIN_WINDOW)
widget.show()

try:
    sys.exit(app.exec_())
except:
    print("Closing the application")