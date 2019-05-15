
#!/bin/sh
#bmp header locate
HEADERFILE=../bmpheader.h
#picture locate
BMPPICTURE=../picture


#产生并且清空

echo "" >  $HEADERFILE

echo "#ifndef _BMPHEADER_H"  >>  $HEADERFILE
echo "#define _BMPHEADER_H"  >>  $HEADERFILE

echo  "" >> $HEADERFILE
echo  "" >> $HEADERFILE
echo  "" >> $HEADERFILE

NUM=0
for i in `ls $BMPPICTURE`
do
    let "NUM++"
done

echo "int BMPNUM = $NUM ; "  >>  $HEADERFILE

echo  "" >> $HEADERFILE
echo  "" >> $HEADERFILE
echo  "" >> $HEADERFILE

echo "unsigned int BMPDATA[] = { "    >> $HEADERFILE

echo  "" >> $HEADERFILE

for i in `ls $BMPPICTURE`
do

./encode   $BMPPICTURE/$i    $HEADERFILE
#echo  "$BMPPICTURE/$i    $HEADERFILE"

done

echo  "" >> $HEADERFILE

echo "};"                                    >> $HEADERFILE

echo  "" >> $HEADERFILE
echo  "" >> $HEADERFILE
echo  "" >> $HEADERFILE





echo  "" >> $HEADERFILE
echo  "" >> $HEADERFILE
echo  "" >> $HEADERFILE


echo "#endif"                >>  $HEADERFILE



