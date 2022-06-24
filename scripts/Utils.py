import requests
import sys
import getopt
import time

from fake_useragent import UserAgent

def DownloadFile(url, filepath):
    with open(filepath, 'wb') as file:
        
        ua = UserAgent()
        headers = {'User-Agent': ua.chrome}
        
        print('Waiting for response...')
        try:
            response = requests.get(url, headers=headers, stream=True)
            total = response.headers.get('content-length')
            print('Downloading...')
        except:
            pass
        
        if total is None:
            file.write(response.content)
        else:
            downloaded = 0
            total = int(total)
            startTime = time.time()
            for data in response.iter_content(chunk_size=max(int(total / 1000), 1024 * 1024)):
                downloaded += len(data)
                file.write(data)
                done = int(50 * downloaded / total)
                percentage = (downloaded / total) * 100
                elapsedTime = time.time() - startTime
                avgKBPerSecond = (downloaded / 1024) / elapsedTime
                avgSpeedString = '{:.2f} KB/s'.format(avgKBPerSecond)
                if (avgKBPerSecond > 1024):
                    avgMBPerSecond = avgKBPerSecond / 1024
                    avgSpeedString = '{:.2f} MB/s'.format(avgMBPerSecond)
                sys.stdout.write('\r[{}{}] {:.2f}% ({})     '.format('#' * done, '.' * (50 - done), percentage, avgSpeedString))
                sys.stdout.flush()
    sys.stdout.write('\n')

def YesOrNo():
    while True:
        try:
            reply = str(input('[Y/N]: ')).lower().strip()
            if reply[:1] == 'y':
                return True
            if reply[:1] == 'n':
                return False
        except KeyboardInterrupt:
            quit()
            
def GetCommandLineArgument(argv, options, allowedCharacters, arguments):
    try:
        opts, args = getopt.getopt(argv, allowedCharacters, arguments)
    except getopt.GetoptError as error:
        return False
    
    result = []
    for opt, arg in opts:
        if opt in options:
            result.append(arg)
    
    return result
    
