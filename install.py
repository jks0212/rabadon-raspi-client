#-*- coding: utf-8 -*-

import os, sys, shutil

TARGET = 'rabadon'
TARGET_SERVICE = 'rabadon-client.service'
TARGET_CONFIG = TARGET + '.conf'
TARGET_SETTINGS = 'settings.json'
PID_FILE = '/run/' + TARGET + '.pid'
CONFIG_DIR = 'rabadon-config'

EXEC_PATH = '/usr/local/bin/'
CONFIG_PATH = '/usr/local/etc/'
SERVICE_PATH = '/lib/systemd/system/'
SYMB_PATH = '/etc/systemd/system/multi-user.target.wants/'
CURR_PATH = os.getcwd() + '/'

def install():
	try:
		shutil.copy(CURR_PATH + TARGET, EXEC_PATH + TARGET)
		print('Execute file copied.')
	except Exception as e:
		err_msg = 'Fail to copy execute file.\n'\
				+ '	-> ' + str(e)
		print(err_msg)

	try:
		shutil.copy(CURR_PATH + TARGET_SERVICE, SERVICE_PATH + TARGET_SERVICE)
		print('Service file copied.')
	except Exception as e:
		err_msg = 'Fail to copy service file.\n'\
				+ '	-> ' + str(e)
		print(err_msg)

	try:
		os.symlink(SERVICE_PATH + TARGET_SERVICE, SYMB_PATH + TARGET_SERVICE)
		print('Symbolic service file linked')
	except Exception as e:
		if e.errno == 17:
			print('Symbolic service file already exists.')
		else:
			err_msg = 'Fail to link symbolic service file.\n'\
					+ '	-> ' + str(e)
			print(err_msg)

	try:
		shutil.copytree(CONFIG_DIR, CONFIG_PATH + CONFIG_DIR)
		print('Config directory copied.')
	except Exception as e:
		if e.errno == 17:
			print('Config directory already exists.')
		else:
			err_msg = 'Fail to copy config directory.\n'\
				+ '	-> ' + str(e)
			print(err_msg)
		
	print('done')
	
def uninstall():
	try:
		os.remove(EXEC_PATH + TARGET)
		print('Execute file removed.')
	except Exception as e:
		if e.errno == 2:
			print('Execute file does not exist.')
		else:
			err_msg = 'Fail to remove execute file.\n'\
					+ '	-> ' + str(e)
			print(err_msg)

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

	try:
		shutil.rmtree(CONFIG_PATH + CONFIG_DIR)
		print('Config file removed.')
	except Exception as e:
		if e.errno == 2:
			print('Config file does not exist.')
		else:
			err_msg = 'Fail to remove config file.\n'\
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

