
#!/bin/sh
#bmp header locate
BINFILE=BMP.bin
#picture locate
BMPPICTURE=../picture


#产生并且清空

if test -f $BINFILE
then
    rm -rf $BINFILE
fi

touch $BINFILE

for i in `ls $BMPPICTURE`
do
./encodebin   $BMPPICTURE/$i    $BINFILE
done


