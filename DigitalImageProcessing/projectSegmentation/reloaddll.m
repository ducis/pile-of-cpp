if libisloaded('FastDLL')
    unloadlibrary('FastDLL')
end
system('copycppfiles')
loadlibrary('FastDLL','FastDLL')
