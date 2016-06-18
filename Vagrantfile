# -*- mode: ruby -*-
# vi: set ft=ruby :

# Vagrantfile API/syntax version. Don't touch unless you know what you're doing!
VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  config.vm.box = "ubuntu/trusty64"

  config.vm.provider "virtualbox" do |vb|
    vb.customize ["modifyvm", :id, "--memory", "8192"]
    vb.customize ["modifyvm", :id, "--cpus", "4"]

#    file_to_disk = './tmp/large_disk.vdi'

#    unless File.exist?(file_to_disk)
#      vb.customize ['createhd', '--filename', file_to_disk, '--size', 100 * 1024]
#    end
#    vb.customize ['storageattach', :id, '--storagectl', 'SATAController', '--port', 1, '--device', 0, '--type', 'hdd', '--medium', file_to_disk]
  end

  config.vm.provision "shell", path: "provision.sh"
end
