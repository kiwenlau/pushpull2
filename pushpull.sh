#!/bin/bash

# transfer docker image from local host to remote host

# step 1: push docker image from local host to the docker-registry, which runs in docker container of remote host
# step 2: pull docker image from the docker-registry

# get the remote host username from the first argument
username=$1

# get the remote host ip from the second argument
remoteip=$2

# get the image name from the third argument
imagename=$3

# set registry version
registryversion=0.9.1
#registryversion=2.0.1

# set image tag
imagetag=latest
#imagetag=flat

# set pass word
password=S0924@0323s

# set local ip
localip=136.187.101.80

# set the address of docker-registry
registryaddress=$localip:5000

# the function for listing local images
list_local_images()
{
    echo -e "\ndocker images"
    docker images | grep $imagename | grep $imagetag
}

# the function for listing remote images
list_remote_images()
{
	 echo -e "\nssh $username@$remoteip \"docker images | grep $imagename\""
	 ssh $username@$remoteip "docker images | grep $imagename | grep $imagetag"
}

#list local images
list_local_images

# change the tag of image
echo -e "\ndocker tag kiwenlau/$imagename:$imagetag $registryaddress/$imagename:$imagetag"
docker tag kiwenlau/$imagename:$imagetag $registryaddress/$imagename:$imagetag

#list local images
list_local_images

# run docker-registry in the container with data volume on remoteip
echo -e "\ndocker run --name=\"docker_registry\" -p 5000:5000 -v /etc/localtime:/etc/localtime:ro -v ~/registry:/tmp/registry -d kiwenlau/registry:$registryversion"
docker rm -f docker_registry &>/dev/null
docker run --name="docker_registry" -p 5000:5000 -v ~/registry:/tmp/registry -d kiwenlau/registry:$registryversion &>/dev/null
docker ps | grep registry

# step 1: push docker image from local host to the docker-registry, which runs in docker container of local host
echo -e "\ndocker push $registryaddress/$imagename:$imagetag"
sleep 2
/usr/bin/time -f "\nreal  %e"  docker push $registryaddress/$imagename:$imagetag 2>pushtime
if [ $? == 0 ]
then
	cat pushtime
else
	# untag local image
	docker rmi $registryaddress/$imagename:$imagetag &>/dev/null
	# delete docker-registry container
	echo -e "\ndocker rm -f docker_registry"
	docker rm -f docker_registry &>/dev/null
	docker ps | grep registry
	echo ""
	#delete data volume
        echo $password | sudo -S rm -rf ~/registry &>/dev/null
	echo ""
	exit 60
fi	

# untag local image
docker rmi $registryaddress/$imagename:$imagetag &>/dev/null

#list remote images
list_remote_images

# step 2: pull docker image from the docker-registry
echo -e "\ndocker pull $registryaddress/$imagename:$imagetag"
ssh $username@$remoteip "/usr/bin/time -f \"\nreal  %e\"  docker pull $registryaddress/$imagename:$imagetag"

#list remote images
list_remote_images

# delete remote image
echo -e "\nssh $username@$remoteip \"docker rmi $registryaddress/$imagename:$imagetag\""
ssh $username@$remoteip "docker rmi $registryaddress/$imagename:$imagetag &>/dev/null"

# list remote images
echo -e "ssh $username@$remoteip \"docker images | grep $imagename\""
ssh $username@$remoteip "docker images | grep $imagename"

# save registry logs
mkdir logs &> /dev/null
docker logs -t docker_registry &>> logs/$imagename.txt
echo -e "\n\n\n" >> logs/$imagename.txt

# delete docker-registry container
echo -e "\ndocker rm -f docker_registry"
docker rm -f docker_registry &>/dev/null
docker ps | grep registry

#delete data volume
echo $password | sudo -S rm -rf ~/registry &>/dev/null
echo ""

rm pushtime
