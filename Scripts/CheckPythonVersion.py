import subprocess
import pkg_resources

def install(package):
    print(f"Installing {package} module...")
    try:
        subprocess.check_call(['python', '-m', 'pip', 'install', package])
    except:
        print('Failed to install ', package, '. You have to install it manually.')

def ValidatePackage(package):
    required = { package }
    installed = {pkg.key for pkg in pkg_resources.working_set}
    missing = required - installed

    if missing:
        print('Detected that package ', package, ' is missing. Trying to install it automatically with pip.')
        install(package)

def ValidatePackages():
    ValidatePackage('requests')
    ValidatePackage('fake-useragent')
