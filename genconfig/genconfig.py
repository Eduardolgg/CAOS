#!/usr/bin/python3

#
# CAOS config files generator
#
#    15-Jan-2019 Elgg
#
#    This file is part of the CAOS init suite,
#    Copyright (C) 2017 Eduardo L. García Glez.
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, you can get a copy at
#    <http://www.gnu.org/licenses/>
#

#
# For details about lsb init scripts see http://refspecs.linuxfoundation.org/LSB_3.1.0/LSB-Core-generic/LSB-Core-generic/tocsysinit.html
#

import os
import sys

import re
import hashlib

class ScriptConfig:
    title = ''

    signature = ''
    name = ''

    lsb_url = 'http://refspecs.linuxfoundation.org/LSB_3.1.0/LSB-Core-generic/LSB-Core-generic/tocsysinit.html'
    short_description = ''
    description = ''

    user_interactive = 'false'

    default_start = []
    default_stop = []

    provides = []
    required_start = []
    required_stop = []

    def __init__(self, scriptName, signature):
        self.title = scriptName.capitalize() + ' caos init config file'
        self.name = scriptName
        self.signature = signature

    def getStringFromStringArray(self, stringArray):
        result = '"' + '", "'.join(str(e) for e in stringArray) + '"'
        return result if len(result) > 2 else ''

    def toString(self):
        result = ''
        result += 'title = "' + self.title + '"\n'
        result += '\n';
        result += '[init-script]\n'
        result += 'signature = "' + self.signature + '"\n'
        result += 'name = "' + self.name + '"\n'
        result += '\n'
        result += '[lsb]\n'
        result += 'lsb-url = "' + self.lsb_url + '"\n'
        result += 'short-description = "' + self.short_description + '"\n'
        result += 'description = "' + self.description + '"\n'
        result += '\n'
        result += 'user-interactive = ' + self.user_interactive + '\n'
        result += '\n'
        result += 'default-start = [ ' + ', '.join(str(e) for e in self.default_start) + ' ]\n'
        result += 'default-stop = [ ' + ', '.join(str(e) for e in self.default_stop) + ' ]\n'
        result += '\n'
        result += 'provides = [ ' + self.getStringFromStringArray(self.provides) + ' ]\n'
        result += 'required-start = [ ' + self.getStringFromStringArray(self.required_start) + ' ]\n'
        result += 'required-stop = [ ' + self.getStringFromStringArray(self.required_stop) + ' ]\n'
        return result

class ConfigReader:

    def __init__(self, filePath, scriptConfig):
        self.filePath = filePath
        self.scriptConfig = scriptConfig

    def gotoLsbHeader(self, file):
        headerStartRe = re.compile("[\s]*(###[\s]*BEGIN[\s]*INIT[\s]*INFO)[\s]*")
        for line in file:
            if headerStartRe.match(line):
                break

    def extractLineConfigInfo(self, line):
        facilityRe = re.compile("[#]{1}[\s]+([a-zA-Z]{1}[\w|\W]+){1}:[\s]*([\w|\W]*)")
        group = facilityRe.findall(line)
        try:
            keyword = group[0][0].upper().strip()
            facilities = group[0][1].strip()

            if keyword == "PROVIDES":
                self.scriptConfig.provides = facilities.split(' ')
            elif keyword == "REQUIRED-START":
                self.scriptConfig.required_start = facilities.split(' ')
            elif keyword == "REQUIRED-STOP":
                self.scriptConfig.required_stop = facilities.split(' ')
            elif keyword == "DEFAULT-START":
                self.scriptConfig.default_start = facilities.split(' ')
            elif keyword == "DEFAULT-STOP":
                self.scriptConfig.default_stop = facilities.split(' ')
            elif keyword == "SHORT-DESCRIPTION":
                self.scriptConfig.short_description = facilities
            elif keyword == "DESCRIPTION": #TODO description could be in two lines
                self.scriptConfig.description = facilities
            elif keyword == "X-INTERACTIVE":
                print("\t" + keyword + ": " + facilities)
                self.scriptConfig.user_interactive = facilities.lower()
            else:
                print("Unknown keyword [" + keyword + "] values [" + facilities + "]")
        except:
            pass

    def extractConfig(self, file):
        headerEndRe = re.compile("[\s]*(###[\s]*END[\s]*INIT[\s]*INFO)[\s]*")
        for line in file:
            if headerEndRe.match(line):
                break
            else:
                self.extractLineConfigInfo(line)

    def readConfig(self):
        file = open(self.filePath + self.scriptConfig.name, "r")
        self.gotoLsbHeader(file)
        self.extractConfig(file)
        file.close();
        return self.scriptConfig

class ConfigWriter:
    def __init__(self, config, scriptDir, outputDir):
        self.config = config
        self.scriptDir = scriptDir
        self.outputDir = outputDir
        self.interactiveScripsDir = ""
        self.noInteractiveScripsDir = ""

    def createConfigTree(self):
        self.interactiveScripsDir = self.outputDir + "interactive"
        if (not os.path.exists(self.interactiveScripsDir)):
            os.makedirs(self.interactiveScripsDir, 0o755)

        self.noInteractiveScripsDir = self.outputDir + "no_interactive"
        if (not os.path.exists(self.noInteractiveScripsDir)):
            os.makedirs(self.noInteractiveScripsDir, 0o755)

    def addInteractiveScript(self):
        symlinkDestInteractive = self.interactiveScripsDir + '/' + self.config.name
        symlinkDestNoInteractive = self.noInteractiveScripsDir + '/' + self.config.name
        if (self.config.user_interactive == 'true' and not os.path.islink(symlinkDestInteractive)):
            os.symlink(self.scriptDir + self.config.name, symlinkDestInteractive)
            print(symlinkDestInteractive + ": " + self.config.user_interactive)
        elif (self.config.user_interactive == 'false' and not os.path.islink(symlinkDestNoInteractive)):
            os.symlink(self.scriptDir + self.config.name, symlinkDestNoInteractive)
            print(symlinkDestNoInteractive + ": " + self.config.user_interactive)

    def writeConfig(self):
        self.createConfigTree();
        self.addInteractiveScript();
        fileName = self.config.name.replace(".sh", ".toml") if self.config.name.endswith('.sh') else self.config.name + '.toml'
        file = open(self.outputDir + fileName, "w")
        file.write("# This file was generated automatically, if you edit it,\n")
        file.write("# your changes can be overwritten.\n\n")
        file.write(self.config.toString())
        file.close();

class Signer:
    def __init__(self, fileNameAndPath):
        self.fileNameAndPath = fileNameAndPath

    def sign(self):
        BLOCKSIZE = 64 * 1024;
        signer = hashlib.sha256()
        with open(self.fileNameAndPath, 'rb') as afile:
                buf = afile.read(BLOCKSIZE)
                while len(buf) > 0:
                    signer.update(buf)
                    buf = afile.read(BLOCKSIZE)
        return signer.hexdigest()

class ConfigGenerator:
    def __init__(self, scriptDir, scriptName, outputDir):
        self.scriptName = scriptName
        self.scriptDir = scriptDir
        self.outputDir = outputDir

    def genConfig(self):
        signature = Signer(self.scriptDir + self.scriptName).sign()
        scriptConfig = ScriptConfig(self.scriptName, signature)
        scriptConfig = ConfigReader(self.scriptDir, scriptConfig).readConfig()
        #print(scriptConfig.toString())
        ConfigWriter(scriptConfig, self.scriptDir, self.outputDir).writeConfig()

def usage(executableName):
    print("Welcome to " + executableName)
    print("Usage: " + executableName + " input_dir output_dir\n")
    print("\t* input_dir:\tDirectory that contains init scripts.")
    print("\t* output_dir:\tDestination directory for CAOS config files.\n")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        usage(sys.argv[0])
        exit(1)

    scriptsDir = sys.argv[1] if sys.argv[1].endswith('/') else sys.argv[1] + '/'
    outputDir = sys.argv[2] if sys.argv[2].endswith('/') else sys.argv[2] + '/'

    try:
        if (not os.path.exists(outputDir)):
                os.makedirs(outputDir, 0o755)
    except OSError as e:
        print("I can't create output directory: " + str(e))
        exit(1)

    scripts = []
    for (root, dirs, files) in os.walk(scriptsDir):
        scripts.extend(files)
        break

    for script in scripts:
        print("In script: " + script)
        ConfigGenerator(scriptsDir, script, outputDir).genConfig()
