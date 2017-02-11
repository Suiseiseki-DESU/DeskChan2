import zipfile, os

def zipdir(path, ziph):
    for root, dirs, files in os.walk(path):
        for file in files:
            ziph.write(os.path.join(root, file))

os.chdir('site-packages')
with zipfile.ZipFile('../win32/bin/python36.zip', 'a') as file:
    zipdir('.', file)
