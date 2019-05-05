chatserve:
	dos2unix chatserve.py
	chmod +x chatserve.py

chatclient:
	gcc chatclient.c chatUtilities.c  -o chatclient.exe