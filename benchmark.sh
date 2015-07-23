#!/bin/bash

# pushpull benchmark: transfer several docker images to 136.187.101.71 host
# change username, remote host ip and image list inside the script

# 31 latest images
imagelist=(axle-base sultans-bin haproxy cb-shell dnsutils node-metrics container-metrics ruby-base ipsec multilevel drupal jruby openjdk mono glassfish jenkins-slave quickstart-python exhibitor ubuntu-perl swagger-editor serf dnsmasq gocd-base gocd-agent drill ubuntu-perl-dev devmachine buildpack-runner gcc buildstep gocd-server)

# 29 flat images
#imagelist=(axle-base sultans-bin haproxy cb-shell http-tap etcd shell drupal textract aws-cli-wetty python-venv jruby glassfish jenkins ubuntu-python-falcon baywatch-client exhibitor ruby-mri ubuntu-perl-mojo swagger-editor baywatch gocd-server drill buildpack-runner ubuntu-perl-dev devmachine boatyard gcc hadoop-docker)

#imagelist=(axle-base gocd-server)

rm -rf output logs > /dev/null
mkdir output

for imagename in ${imagelist[*]};
do
	echo -e "\n./run.sh kai 136.187.101.71 $imagename"
		file="output/$imagename.txt"
	./run.sh kai 52.68.197.112 $imagename 2>&1 | tee $file
done
