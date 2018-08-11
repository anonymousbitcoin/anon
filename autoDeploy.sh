while [ 1 ]; do
	git fetch
	status=$(git status)
	if [[ $status = *"up-to-date"* ]]; then
        	echo Anon up-to-date
	else
		echo ----------------
        	echo  Killing server
		echo ----------------
	        ./src/anon-cli -testnet stop
		echo ----------------
	        echo Anon repo is not up to date. Updating anon repo...
		echo ----------------
	        git pull origin live-nodes
		echo ----------------
	        echo Building anon node
		echo ----------------
	        ./anonutil/build.sh -j$(nproc)
		echo ----------------
       		echo Fetching Params
		echo ----------------
        	./anonutil/fetch-params.sh
		echo ----------------
		echo deleting testnet folder
		echo ----------------
		rm -rf ../.anon/testnet4
		echo ----------------
        	echo Running ANON daemon
		echo ----------------
        	./src/anond -testnet -reindex
		echo ----------------
        	echo PROCESS FINISHED
		echo ----------------
	fi
	sleep 10
done
