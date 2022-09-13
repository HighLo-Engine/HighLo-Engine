import shutil

def CopyFileAndReplaceNamespaceWithProjectName(dst, src, projectName):
    # first read the text content
    with open(src, "r") as fr, open(dst, "w") as fw:
        for line in fr:
            idx = line.find('siteip = ')
            lineStripped = line[: idx] + "siteip = \"%s\"" %(siteip) + "\n" if idx != -1 else line
            
            if 'project_name' in lineStripped:
                # we found the line where we need to replace the project name
                lineStripped = lineStripped.replace('<project_name>', projectName)
                
            fw.write(lineStripped)
            
        fr.close()
        fw.close()
        
        