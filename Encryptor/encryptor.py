import json
import os
import sys
from base64 import b64encode, b64decode
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from Crypto.Cipher import PKCS1_OAEP

KEY_LENGTH = 2048
DEFAULT_PUBLIC_PATH = "../Generate/public.pem"
DEFAULT_PRIVATE_PATH = "../Generate/private.pem"
DEFAULT_META = "../meta.txt"


def bytesToString(key_bytes):
    string = ""
    for byte in key_bytes:
        string += str(byte) + ','
    return string


def stringToBytes(string):
    ints = []
    curr = 0
    for c in string:
        if c == ',':
            ints.append(curr)
            curr = 0
        else:
            val = ord(c) - ord('0')
            curr *= 10
            curr += val
    return bytes(ints)


def getMetaPath(path: str, file: str):
    index_p = path.rindex('/')
    index_f = file.rindex('/')
    path_base = path[:index_p+1]
    file_name = file[index_f+1:]

    index_f = file_name.rindex('.')
    file_name = file_name[:index_f]

    path = path_base + "meta/" + file_name + "_meta.txt"
    return path


def SymmetricEncode(meta_path, data, public_key):
    d = bytes(data, 'utf-8')
    session_key = get_random_bytes(16)
    cipher = AES.new(session_key, AES.MODE_CTR)
    ct_bytes = cipher.encrypt(d)

    nonce = b64encode(cipher.nonce).decode('utf-8')
    ct = b64encode(ct_bytes).decode('utf-8')

    cipher_rsa = PKCS1_OAEP.new(public_key)
    enc_key = cipher_rsa.encrypt(session_key)

    result = json.dumps({'nonce': nonce, 'key': bytesToString(enc_key)})

    file = open(meta_path, "wb")
    file.write(bytes(result, 'utf-8'))
    file.close()
    return ct


def SymmetricDecode(meta_path, data, private_key):
    f = open(meta_path, 'r')
    json_input = f.readline()
    b64 = json.loads(json_input)

    nonce = b64decode(b64['nonce'])
    enc_key = b64['key']
    enc_key = stringToBytes(enc_key)

    cipher_rsa = PKCS1_OAEP.new(private_key)
    session_key = cipher_rsa.decrypt(enc_key)

    cipher = AES.new(session_key, AES.MODE_CTR, nonce=nonce)
    data = b64decode(data)
    pt = cipher.decrypt(data)
    return pt


def Encryption(file_path, public_key_path):
    f = open(public_key_path, 'r')
    public_key = RSA.import_key(f.read())
    print(public_key)
    print()

    file = open(file_path, 'r')
    data = ""
    lines = file.readlines()
    for line in lines:
        data += line

    meta_path = getMetaPath(public_key_path, file_path)
    encrypted = SymmetricEncode(meta_path, data, public_key)

    file = open(file_path, "wb")
    file.write(bytes(encrypted, 'utf-8'))
    file.close()


def Decryption(file_path, private_key_path):
    f = open(private_key_path, 'r')
    private_key = RSA.import_key(f.read())
    print(private_key)
    print()

    file = open(file_path, 'r')
    data = ""
    lines = file.readlines()
    for line in lines:
        data += line

    meta_path = getMetaPath(private_key_path, file_path)
    decrypted = SymmetricDecode(meta_path, data, private_key)

    file = open(file_path, "wb")
    file.write(decrypted)
    file.close()

    os.remove(meta_path)
