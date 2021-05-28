import sys
import os
from PyQt5.uic import loadUi
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QDialog, QApplication, QWidget, QFileDialog
from PyQt5.QtGui import QPixmap
from Encryptor.encryptor import Encryption, Decryption


def encrypt(key_path, file_path):

    Encryption(file_path, key_path)
    print("Encryption\nkey:{}\nfile:{}".format(key_path, file_path))


def decrypt(key_path, file_path):
    Decryption(file_path, key_path)
    print("Decryptiuon\nkey:{}\nfile:{}".format(key_path, file_path))

def load_public_keys(path):

    cwd = (os.getcwd()).replace(os.sep, '/')
    dict = {}
    arr = os.listdir(path)

    for key in arr:
        key_name = os.path.splitext(key)[0]
        dict[key_name] = cwd+"/PublicKeys/"+key

    return dict

