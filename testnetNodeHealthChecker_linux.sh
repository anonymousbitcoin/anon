#!/bin/bash

while [ 1 ]; do
	git fetch origin
	sleep 5
	status=$(git status)
	if [[ $status = *"up to date"* ]]; then
		# echo Anon up-to-date
	else
		echo Stopping ANON Node
		./src/anon-cli -testnet stop
		sleep 5
		echo Anon repo is not up to date. Updating anon repo...
		git pull origin master
		sleep 5
		echo Rebuilding anon node
		./anonutil/build.sh
		echo Fetching Params
		sleep 5
		./anonutil/fetch-params.sh
		echo Running ANON daemon
		./src/anond -testnet -reindex -daemon
	fi
	sleep 300

done
