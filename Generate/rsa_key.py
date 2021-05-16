from Crypto.PublicKey import RSA
import sys

KEY_LENGTH = 2048
DEFAULT_PUBLIC_PATH = "Generate/public.pem"
DEFAULT_PRIVATE_PATH = "Generate/private.pem"

if __name__ == "__main__":
    if len(sys.argv) < 3:
        public_path = DEFAULT_PUBLIC_PATH
        private_path = DEFAULT_PRIVATE_PATH
    else:
        public_path = sys.argv[1]
        private_path = sys.argv[2]

    # Generowanie pary kluczy
    key = RSA.generate(KEY_LENGTH)

    # Zapisanie w plikach
    public_f = open(public_path, "wb")
    public_f.write(key.public_key().export_key('PEM'))
    public_f.close()
    print("Klucz publiczny zapisano w: ", public_path)

    private_f = open(private_path, "wb")
    private_f.write(key.export_key('PEM'))
    private_f.close()
    print("Klucz prywatny zapisano w: ", private_path)
