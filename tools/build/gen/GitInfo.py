#!/usr/bin/env python

import subprocess

# Class representation of Git version information
class RevisionInformation:

	"""Do not use this function in any user code."""
	@staticmethod
	def __output(command):
		try:
			return subprocess.run(command, shell=True, capture_output=True, text=True, check=True).stdout.replace('\n', '')
		except:
			# This isn't very graceful, but it works when starting a project
			# with clean Git history. After a initial commit, this will go away
			# and everything will work nice and happy, like nothing ever happened.
			return "unknown"
	
	def __init__(self, trunk='master', dirtyChar='~'):
		self.__dirtyChar = dirtyChar
		self.__trunk = trunk
		
		self.commit = RevisionInformation.__output("git rev-parse --short HEAD")
		self.branch = RevisionInformation.__output("git rev-parse --abbrev-ref HEAD")
		self.tag = RevisionInformation.__output(f"git describe --tags --always --dirty={self.__dirtyChar}")

		self.trunkBuild = self.branch == self.__trunk or self.branch == "HEAD"
		self.dirty = self.tag.endswith(self.__dirtyChar)
		if self.dirty:
			self.commit += self.__dirtyChar
		self.hasTag = self.commit != self.tag

		self.fullTag = self.getVersionTag()

	# Get a version tag.
	def getVersionTag(self):
		out = ""
		if self.trunkBuild:
			# we don't care if this is just a commit or is actually a tag
			# if it's dirty it'll give itself the tilde and everything
			out += self.tag
		else:
			# we do care here, however
			if self.hasTag:
				out += f"{self.tag}{'' if self.dirty else '-'}"

			out += f"{self.commit}{'' if self.dirty else '-'}"
			out += self.branch
		return out
		
		
"""Interface for generating Git information files."""
class OutputProcessor:
	""" Returns the fully formatted output. """
	def getOutput(self, info):
		pass
		