Imports System.IO
Imports Newtonsoft.Json

Module Helpers
    Public Class JSON_Config_Change
        Public offset As String
        Public patch As String
    End Class

    Public Class JSON_Config_Patch
        Public name As String
        Public apply As String

        <JsonProperty(NullValueHandling:=NullValueHandling.Ignore)>
        Public changes As List(Of JSON_Config_Change)

        <JsonIgnore>
        Public customized As Boolean = False
    End Class
    Public Class JSON_Config
        <JsonProperty(PropertyName:="custom-config", NullValueHandling:=NullValueHandling.Ignore)>
        Public customconfig As String

        <JsonProperty(NullValueHandling:=NullValueHandling.Ignore)>
        Public lua As String

        <JsonProperty(NullValueHandling:=NullValueHandling.Ignore)>
        Public patches As List(Of JSON_Config_Patch)

        Public Function FindPatchByName(name As String)
            FindPatchByName = Nothing
            For Each item In patches
                If item.name IsNot Nothing AndAlso item.name = name Then
                    FindPatchByName = item
                End If
            Next
        End Function

    End Class

    Public Function LoadMergedJSON(jsonpath As String)

        ' Load the base JSON file
        Dim jsontext As String = File.ReadAllText(jsonpath + "\FTSE_config_base.json")
        Dim obj As JSON_Config
        obj = JsonConvert.DeserializeObject(Of JSON_Config)(jsontext)

        ' Load the custom config file, if specified
        If obj.customconfig IsNot Nothing Then

            Try
                Dim jsontext2 As String = File.ReadAllText(jsonpath + "\" + obj.customconfig)
                Dim obj2 As JSON_Config
                obj2 = JsonConvert.DeserializeObject(Of JSON_Config)(jsontext2)

                For Each mergeitem In obj2.patches

                    ' Change the name of the "Burst bug fix" to show that it's the old hex patch
                    If mergeitem.name = "Burst bug fix" Then
                        mergeitem.name = "Old burst bug fix"
                    End If

                    ' Find the same patch in the base file, if it exists
                    Dim targetitem As JSON_Config_Patch = obj.FindPatchByName(mergeitem.name)
                    If targetitem Is Nothing Then
                        ' Base file doesn't have this patch, add the whole thing
                        mergeitem.customized = True
                        obj.patches.Add(mergeitem)
                    Else
                        ' Base file has this patch

                        ' Update the apply flag
                        If mergeitem.apply IsNot Nothing Then
                            targetitem.apply = mergeitem.apply
                        End If

                        ' Check if the changes match
                        Dim difference As Boolean = False
                        If mergeitem.changes IsNot Nothing Then
                            For i As Integer = 0 To mergeitem.changes.Count - 1
                                If targetitem.changes(i) Is Nothing OrElse
                                    targetitem.changes(i).offset Is Nothing OrElse
                                    targetitem.changes(i).patch Is Nothing OrElse
                                    (mergeitem.changes(i).offset IsNot Nothing AndAlso mergeitem.changes(i).offset <> targetitem.changes(i).offset) OrElse
                                    (mergeitem.changes(i).patch IsNot Nothing AndAlso mergeitem.changes(i).patch <> targetitem.changes(i).patch) Then
                                    difference = True
                                    MsgBox("Difference! " + targetitem.changes(i).offset + " and " + mergeitem.changes(i).offset)


                                End If
                            Next
                        End If
                        If difference Then
                            ' The customized file has a different patch, keep it
                            targetitem.changes = mergeitem.changes
                            targetitem.customized = True
                        End If
                    End If

                Next

            Catch ex As Exception

            End Try

        End If
        LoadMergedJSON = obj
    End Function

    Public Sub WriteConfigJSON(jsonpath As String, config As JSON_Config)
        For Each item In config.patches
            If Not item.customized Then
                item.changes = Nothing
            End If
        Next

        Dim filename As String = jsonpath + "\" + config.customconfig
        Dim temp = config.customconfig
        Dim temp2 = config.lua

        config.customconfig = Nothing
        config.lua = Nothing

        Dim outstring As String = JsonConvert.SerializeObject(config, Formatting.Indented)

        File.WriteAllText(filename, outstring, System.Text.Encoding.ASCII)

        config.customconfig = temp
        config.lua = temp2

    End Sub

    Private Function CheckResource(compare As Byte(), res As Byte())
        CheckResource = False
        If UBound(compare) <> UBound(res) Then
            Exit Function
        End If
        For i = LBound(compare) To UBound(compare)
            If compare(i) <> res(i) Then
                Exit Function

            End If
        Next i
        CheckResource = True
    End Function
    Enum ExecutableStatus
        UNPATCHED
        PATCHED
        UNKNOWN
        NOT_FILE
    End Enum

    Function CheckExecutableStatus(bosname As String)
        On Error GoTo EndFxn
        CheckExecutableStatus = ExecutableStatus.NOT_FILE
        Dim bosexe As New System.IO.FileStream(bosname, FileMode.Open)
        Dim section1(48) As Byte
        Dim section2(15) As Byte
        Dim section3(3) As Byte

        Using bosreader As New BinaryReader(bosexe)
            bosexe.Position = &H2C438B
            section1 = bosreader.ReadBytes(48)
            bosexe.Position = &H2BE2F1
            section2 = bosreader.ReadBytes(15)
            bosexe.Position = &H2BE32D
            section3 = bosreader.ReadBytes(3)
        End Using

        If CheckResource(section1, My.Resources.patch1) AndAlso
                CheckResource(section2, My.Resources.patch2) AndAlso
                CheckResource(section3, My.Resources.patch3) Then
            CheckExecutableStatus = ExecutableStatus.PATCHED
        ElseIf CheckResource(section1, My.Resources.source1) AndAlso
                CheckResource(section2, My.Resources.source2) AndAlso
                CheckResource(section3, My.Resources.source3) Then
            CheckExecutableStatus = ExecutableStatus.UNPATCHED
        Else
            CheckExecutableStatus = ExecutableStatus.UNKNOWN
        End If

EndFxn:
        MsgBox(Err.Description, vbOKOnly Or vbExclamation, "FTSESetup error reading file")
    End Function

    Public Sub PatchFile(bosname As String, patch1 As Byte(), patch2 As Byte(), patch3 As Byte())
        Dim bosexe As New System.IO.FileStream(bosname, FileMode.Open)

        Using boswriter As New BinaryWriter(bosexe)
            bosexe.Position = &H2C438B
            boswriter.Write(patch1)
            bosexe.Position = &H2BE2F1
            boswriter.Write(patch2)
            bosexe.Position = &H2BE32D
            boswriter.Write(patch3)
            boswriter.Flush()

        End Using
    End Sub
    Public Sub FlipStatus(bosname As String)
        Dim state As ExecutableStatus

        state = CheckExecutableStatus(bosname)
        If state = ExecutableStatus.UNPATCHED Then
            Try
                My.Computer.FileSystem.CopyFile(
                bosname, bosname + ".ftse.bak", overwrite:=False)
            Catch ex As Exception
                ' Ignore error on backup copy already exists
            End Try

            PatchFile(bosname,
            My.Resources.patch1,
            My.Resources.patch2,
            My.Resources.patch3)
        ElseIf state = ExecutableStatus.PATCHED Then
            PatchFile(bosname,
            My.Resources.source1,
            My.Resources.source2,
            My.Resources.source3)
        End If

    End Sub
    Public Function DefaultEXE()
        Try
            Dim ret As String
            ret = My.Computer.FileSystem.ReadAllText("FTSESetup.ini")
            DefaultEXE = ret
        Catch ex As Exception
            Dim basedir = My.Computer.FileSystem.CurrentDirectory

            If My.Computer.FileSystem.FileExists(basedir + "\BOS_HR.exe") Then
                DefaultEXE = basedir + "\BOS_HR.exe"
            ElseIf My.Computer.FileSystem.FileExists(basedir + "\BOS.exe") Then
                DefaultEXE = basedir + "\BOS.exe"
            Else
                DefaultEXE = ""
            End If
        End Try
    End Function
    Public Sub SetDefaultEXE(exename As String)
        Try
            My.Computer.FileSystem.WriteAllText("FTSESetup.ini", exename, False)
        Catch ex As Exception

        End Try
    End Sub

    Public Sub CheckDefaultConfig(path As String)
        My.Computer.FileSystem.WriteAllBytes(path + "\FTSE_config_base.json", My.Resources.FTSE_config_base, False)
        My.Computer.FileSystem.WriteAllBytes(path + "\ftse_base.lua", My.Resources.ftse_base, False)
        My.Computer.FileSystem.WriteAllBytes(path + "\FTSE.dll", My.Resources.FTSE, False)
        If Not My.Computer.FileSystem.FileExists(path + "\FTSE_config.json") Then
            My.Computer.FileSystem.WriteAllText(path + "\FTSE_config.json", "{ ""patches"": [ ] }", False, System.Text.Encoding.ASCII)
        End If
        If Not My.Computer.FileSystem.FileExists(path + "\FTSE.lua") Then
            My.Computer.FileSystem.WriteAllText(path + "\FTSE.lua", "-- Add customized scripts here.", False, System.Text.Encoding.ASCII)

        End If

    End Sub
End Module
