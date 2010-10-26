#!/bin/sh

echo  "Cronjob Starts [`date`]"

basepath="/var/senderReputation";
indir="${basepath}/out";
processingdir="${basepath}/insertin";
outdir="${basepath}/insertout";
scriptsdir="${basepath}/scripts";
bindir="${basepath}/bin";
insertbin="insertData";
backupMountPoint="/var/backup";

#while [[ 1 > 0 ]] ; do

echo "waking up service ... "

today=`date +%Y%m%d`;

batchfilename=`date +%Y%m%d%H%M%S.$RANDOM`;
batchfilepath="${processingdir}/${batchfilename}";

#echo "LOAD DATA INFILE \"-\" INTO TABLE \"rediffmail_srs\";" >> $batchfilepath;
#echo "#timestamprowcolumnvalue" >> $batchfilepath;

cd $indir/
for file in *;
do
if [ -s $file ] 
    then
    today=`date +%Y%m%d`;
    todaysBackupDir="${backupMountPoint}/${today}";

if [ ! -d $todaysBackupDir ]
    then
    echo "Creating backup folder for today - ${todaysBackupDir}";
mkdir -p "${todaysBackupDir}";
chown rmail:rmail "${todaysBackupDir}/";
fi

myhostname=`hostname`;

cp "${indir}/${file}" "${todaysBackupDir}/${myhostname}_${file}";
mv "${indir}/${file}" "${processingdir}/${file}";
#nohup ${bindir}/${insertbin} "/opt/hypertable/current/conf/hypertable.cfg" "${processingdir}/${file}" "${outdir}" &
#echo ${bindir}/${insertbin} \"/opt/hypertable/current/conf/hypertable.cfg\" \"${processingdir}/${file}\" \"${outdir}\"
#${bindir}/${insertbin} "/opt/hypertable/current/conf/hypertable.cfg" "${processingdir}/${file}" "${outdir}"
/usr/bin/perl ${bindir}/bulk_load.pl "${processingdir}/${file}" >> $batchfilepath
echo "${file} processed";
else
rm -f "${indir}/${file}";
echo "The file ${indir}/${file} has been removed as it is zero byte...";
fi
done

#if [ -s $batchfilepath ]
#then
cd "/var/senderReputation/insertin/"

echo "USE 'rediffmail'; LOAD DATA INFILE '$batchfilename' INTO TABLE srs;exit;" |/opt/hypertable/current/bin/ht shell;
#echo "--------------------------------------------------------------------------------------------" >> /var/log/${today}.inserterr.log;
#fi
rm -f $batchfilepath;
#done

echo  "Cronjob Ends [`date`]"
