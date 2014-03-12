if libisloaded('FastDLL')
    unloadlibrary('FastDLL')
end
system('copydebugcppfiles')
loadlibrary('FastDLL','FastDLL')
