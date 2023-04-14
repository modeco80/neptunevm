#!/usr/bin/env python

from . import GitInfo
from . import OutputProcessorImpl

from enum import Enum

class TargetLanguage(Enum):
	"""C language definitions."""
	C = 0
	"""C++ language constexpr variables."""
	CC = 1
		
def createOutputProcessor(target):
	match target:
		case TargetLanguage.C:
			return OutputProcessorImpl.OutputProcessorC()
		case TargetLanguage.CC:
			return OutputProcessorImpl.OutputProcessorCC()
		case _:
			raise ValueError("Invalid target language")