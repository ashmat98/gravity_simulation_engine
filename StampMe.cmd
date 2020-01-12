@ECHO off
  SETLOCAL
  IF [%1] NEQ [] goto s_start

  :: Author - Simon Sheppard, Nov 2014
  :: Tested for Windows XP Pro or greater
  Echo StampMe.cmd
  Echo Rename a file with the DATE/Time
  Echo:
  Echo Syntax
  Echo    STAMPME TestFile.txt
  Echo:
  Echo    STAMPME "Test File.txt"
  Echo:
  Echo    STAMPME "c:\docs\Test File.txt"
  Echo:
  Echo    Will rename the file in the format "Test File-2014-12-30@16-55-01.txt"
  Echo:
  Echo    In a batch file use CALL STAMPME ...
  GOTO :eof
  
  :s_start
  Set _file=%~n1%
  Set _pathname=%~f1%
  Set _ext=%~x1%
  
  :: Get the date/time
  FOR /f %%a in ('WMIC OS GET LocalDateTime ^| find "."') DO Set _DTS=%%a
  Set _datetime=%_DTS:~0,4%-%_DTS:~4,2%-%_DTS:~6,2%@%_DTS:~8,2%-%_DTS:~10,2%-%_DTS:~12,2%

  :: Echo   Year-MM-Day@HR-Min-Sec
  :: Echo   %_datetime%
  
  :: Rename the file
  REN "%_pathname%" "%_datetime%-%_file%%_ext%"