import subprocess
import pkg_resources

def install(package):
    print(f"Installing {package} module...")
    try:
        subprocess.check_call(['python', '-m', 'pip', 'install', package])
    except:
        pass

def ValidatePackage(package):
    required = { package }
    installed = {pkg.key for pkg in pkg_resources.working_set}
    missing = required - installed

    if missing:
        install(package)

def ValidatePackages():
    ValidatePackage('requests')
    ValidatePackage('fake-useragent')
