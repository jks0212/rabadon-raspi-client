#-*- coding: utf-8 -*-

import os, sys, shutil

TARGET = 'rabadon'
TARGET_SERVICE = 'rabadon-client.service'
TARGET_CONFIG = TARGET + '.conf'
TARGET_SETTINGS = 'settings.json'
PID_FILE = '/run/' + TARGET + '.pid'
CONFIG_DIR = 'config/'

EXEC_PATH = '/usr/local/bin/rabadon/'
SERVICE_PATH = '/lib/systemd/system/'
SYMB_PATH = '/etc/systemd/system/multi-user.target.wants/'
CURR_PATH = os.getcwd() + '/'

"""
	TODO
	 Config directory is in the execute directory.
	 It should be in another directory.
	 But future problem.
"""

def install():
	""" Execute directory """
	try:
		os.mkdir(EXEC_PATH)
		print('Execute directory has been made.')
	except Exception as e:
		if e.errno == 17:
			print('Execute directory already exists.')
		else:
			err_msg = 'Fail to make execute directory.\n'\
					+ '	-> ' + str(e)
			print(err_msg)
	""" Exectue file """
	try:
		shutil.copy(CURR_PATH + TARGET, EXEC_PATH + TARGET)
		print('Execute file has been copied.')
	except Exception as e:
		err_msg = 'Fail to copy execute file.\n'\
				+ '	-> ' + str(e)
		print(err_msg)
	""" Config directory with file """
	try:
		shutil.copytree(CURR_PATH + CONFIG_DIR, EXEC_PATH + CONFIG_DIR)
		print('Config directory has been copied.')
	except Exception as e:
		if e.errno == 17:
			print('Config directory already exists.')
		else:
			err_msg = 'Fail to copy config directory.\n'\
				+ '	-> ' + str(e)
			print(err_msg)
	""" Service file """
	try:
		shutil.copy(CURR_PATH + TARGET_SERVICE, SERVICE_PATH + TARGET_SERVICE)
		print('Service file has been copied.')
	except Exception as e:
		err_msg = 'Fail to copy service file.\n'\
				+ '	-> ' + str(e)
		print(err_msg)
	""" Symbolic file """
	try:
		os.symlink(SERVICE_PATH + TARGET_SERVICE, SYMB_PATH + TARGET_SERVICE)
		print('Symbolic service file has been linked')
	except Exception as e:
		if e.errno == 17:
			print('Symbolic service file already exists.')
		else:
			err_msg = 'Fail to link symbolic service file.\n'\
					+ '	-> ' + str(e)
			print(err_msg)

	print('done')
	
def uninstall():
	""" Execute file """
	try:
		os.remove(EXEC_PATH + TARGET)
		print('Execute file removed')
	except Exception as e:
		if e.errno == 2:
			print('Execute file does not exist')
		else:
			err_msg = 'Fail to remove execute file.\n'\
					+ '	-> ' + str(e)
			print(err_msg)
	""" Service file """
	try:
		os.remove(SERVICE_PATH + TARGET_SERVICE)
		print('Service file removed.')
	except Exception as e:
		if e.errno == 2:
			print('Service file does not exist.')
		else:
			err_msg = 'Fail to remove service file.\n'\
					+ '	-> ' + str(e)
			print(err_msg)
	""" Symbolic file """
	try:
		os.remove(SYMB_PATH + TARGET_SERVICE)
		print('Symbolic service file removed.')
	except Exception as e:
		if e.errno == 2:
			print('Symbolic service file does not exist.')
		else:
			err_msg = 'Fail to remove symbolic service file.\n'\
					+ '	-> ' + str(e)
			print(err_msg)

	print('done')

if __name__ == '__main__':
	if len(sys.argv) == 2:
		param = sys.argv[1]
		if param == 'install':
			install()
		elif param == 'uninstall':
			uninstall()
		else:
			print('Use install or uninstall as a parameter.')
	else:
		print('Use install or uninstall as a parameter.')

