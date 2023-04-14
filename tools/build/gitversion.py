#!/usr/bin/env python

import sys
from gen import GitInfo, OutputProcessor

TARGET_LANGUAGE = OutputProcessor.TargetLanguage.CC
CC_NAMESPACE = "neptunevm::version"
C_PREFIX = "NEPTUNEVM_VERSION"

TRUNK = "master"
DIRTY_CHAR = '*'

def usage(err):
	if err != None:
		print(f"Error: {err}") 
	print(f"Usage: {sys.argv[0]} [path to Git version file]")
	print("Current configuration: ")
	print(f"    Trunk branch: {TRUNK}")
	print(f"    C++ namespace: {CC_NAMESPACE}")
	print(f"    C macro prefix: {C_PREFIX}")
	print(f"    Dirty worktree indicator: {DIRTY_CHAR}")
	if err != None:
		sys.exit(1)
	else:
		sys.exit(0)

def main():
	if not len(sys.argv) > 1:
		usage("Filename argument not provided")

	info = GitInfo.RevisionInformation(TRUNK, DIRTY_CHAR)
	
	# Setup the output processor depending on target language
	outputProcessor = OutputProcessor.createOutputProcessor(TARGET_LANGUAGE)
	match TARGET_LANGUAGE:
		case OutputProcessor.TargetLanguage.C:
			outputProcessor.setPrefix(C_PREFIX)
		case OutputProcessor.TargetLanguage.CC:
			outputProcessor.setNamespace(CC_NAMESPACE)
			outputProcessor.setCPrefix(C_PREFIX)
   
	# Write the file!
	with open(sys.argv[1], "w+") as file:
		file.write(outputProcessor.getOutput(info))

if __name__ == "__main__":
	main()
