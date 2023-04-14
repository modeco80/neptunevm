#!/usr/bin/env python

# This file provides implementation of GitInfo.OutputProcessor for languages we support.

from . import GitInfo

""" Implmentation of OutputProcessor for C language definitions """
class OutputProcessorC(GitInfo.OutputProcessor):
	def __init__(self):
		self.__prefix = ""
		
	def setPrefix(self, prefix):
		self.__prefix = prefix

	def _getOutputWithoutCppControlStatements(self, info):
		return f"""
#define {self.__prefix}_GIT_COMMIT "{info.commit}"
#define {self.__prefix}_GIT_BRANCH "{info.branch}"
#define {self.__prefix}_GIT_TAG "{info.tag}" 
#define {self.__prefix}_GIT_FULL_TAG "{info.fullTag}"

#define {self.__prefix}_GIT_IS_DIRTY {OutputProcessorC.__bool_to_c(info.dirty)}
#define {self.__prefix}_GIT_IS_TRUNK {OutputProcessorC.__bool_to_c(info.trunkBuild)}
		"""
	
	def getOutput(self, info):
		return f"""
/* This is mostly a test */
#ifndef {self.__prefix}_VERSION_H
#define {self.__prefix}_VERISON_H
	{self._getOutputWithoutCppControlStatements(info)}
#endif
		"""

	@staticmethod
	def __bool_to_c(value: bool):
		if value:
			return "1"
		return "0"
		
		
""" Implmentation of OutputProcessor for C++ """
class OutputProcessorCC(GitInfo.OutputProcessor):
	def __init__(self):
		# We also provide C definitions when using C++ as the output language. 
		# To avoid repeating code generation, we use the C implementation.
		self.__cprocessor = OutputProcessorC()
		self.__namespace = ""
		
	def setNamespace(self, namespace):
		self.__namespace = namespace;
		
	def setCPrefix(self, prefix):
		self.__cprocessor.setPrefix(prefix)

	@staticmethod
	def __bool_to_cc(value: bool):
		return str(value).lower()
	
	def getOutput(self, info):
		return f"""
// This file is generated, do not edit it.

#pragma once

{self.__cprocessor._getOutputWithoutCppControlStatements(info)}

#ifdef __cplusplus
namespace {self.__namespace} {{
	inline constexpr char commit[] = "{info.commit}";
	inline constexpr char branch[] = "{info.branch}";
	inline constexpr char tag[] = "{info.tag}";
	inline constexpr char fullTag[] = "{info.fullTag}";
	
	inline constexpr bool isDirty = {OutputProcessorCC.__bool_to_cc(info.dirty)};
	inline constexpr bool isTrunk = {OutputProcessorCC.__bool_to_cc(info.trunkBuild)};
}}
#endif
		"""
