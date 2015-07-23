#!/bin/bash
# run pushpull.sh for 10 times

# get the remote host username from the first argument
username=$1

# get the remote host ip from the second argument
remoteip=$2

# get the image name from the third argument
imagename=$3

# the function for listing local images
list_images()
{
    echo -e "\nsudo docker images"
    docker images | grep $imagename
}

echo -e "\npullpush benchmark: transfer $imagename image to $remoteip host\n "

# print hostname
echo -e "hostname: \c" ; hostname

# print begin date
echo -e "begin date: \c" ; date

#list local images
list_images

# pull image from Docker Hub
#echo -e "\nsudo docker pull kiwenlau/$imagename:latest"
#sudo docker pull kiwenlau/$imagename:latest >>/dev/null

#list local images
list_images

echo ""

cmd="./pushpull.sh $username $remoteip $imagename"

i=1
while [ $i -le 10 ]
do
  echo -e "\nTest $i: $cmd"
  $cmd
	if [ $? != 60 ]
	then
		((i++))
	fi
done

# print end date
echo -e "end date: \c" ; date
echo -e "\n"
