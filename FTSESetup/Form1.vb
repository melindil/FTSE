
Imports FTSESetup.Helpers
Public Class Form1
    Dim config As Helpers.JSON_Config

    Private Function CheckUnsavedChanges()
        CheckUnsavedChanges = False
        If Button3.Enabled = True Then
            Dim ret As MsgBoxResult = MsgBox("This action will reset any unsaved changes to the hex patch list. Proceed?", MsgBoxStyle.OkCancel)
            If ret = MsgBoxResult.Cancel Then
                CheckUnsavedChanges = True
            End If
        End If
    End Function
    Private Sub Form1_SetExecutableState()
        Dim exestatus As ExecutableStatus
        exestatus = CheckExecutableStatus(TextBox1.Text)

        If exestatus = ExecutableStatus.PATCHED Then
            Label3.Text = "INSTALLED"
            Label3.ForeColor = Color.LimeGreen
            Button2.Enabled = True
            Button2.Text = "Uninstall"
            Helpers.SetDefaultEXE(TextBox1.Text)
            Helpers.CheckDefaultConfig(My.Computer.FileSystem.GetParentPath(TextBox1.Text))
            config = Helpers.LoadMergedJSON(My.Computer.FileSystem.GetParentPath(TextBox1.Text))
            PopulateListbox()
            CheckedListBox1.Enabled = True
            Button3.Enabled = False
            Button4.Enabled = True



        ElseIf exestatus = ExecutableStatus.UNPATCHED Then
            Label3.Text = "UNINSTALLED"
            Label3.ForeColor = Color.Gray
            Button2.Enabled = True
            Button2.Text = "Install"
            Helpers.SetDefaultEXE(TextBox1.Text)
            config = Nothing
            CheckedListBox1.Items.Clear()
            CheckedListBox1.Enabled = False
            Button3.Enabled = False
            Button4.Enabled = False
        ElseIf exestatus = ExecutableStatus.NOT_FILE Then
            Label3.Text = ""
            Label3.ForeColor = Color.Gray
            Button2.Enabled = False
            Button2.Text = "Install"
            CheckedListBox1.Items.Clear()
            CheckedListBox1.Enabled = False
            Button3.Enabled = False
            Button4.Enabled = False
            config = Nothing
        Else
            Label3.Text = "UNKNOWN"
            Label3.ForeColor = Color.Red
            Button2.Text = "Install"
            Button2.Enabled = False
            CheckedListBox1.Items.Clear()
            CheckedListBox1.Enabled = False
            Button3.Enabled = False
            Button4.Enabled = False
            config = Nothing
        End If
    End Sub
    Private Sub PopulateListbox()
        If config Is Nothing Then
            Exit Sub
        End If
        For Each patch In config.patches
            CheckedListBox1.Items.Add(patch.name, (patch.apply = "true"))
        Next
    End Sub

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles Me.Load
        TextBox1.Text = Helpers.DefaultEXE()


        Form1_SetExecutableState()


    End Sub


    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If CheckUnsavedChanges() Then
            Exit Sub
        End If
        Dim fd As OpenFileDialog = New OpenFileDialog()

        fd.Title = "Select BOS Executable"
        fd.Filter = "EXE files (*.EXE)|*.EXE|All files (*.*)|*.*"
        fd.FilterIndex = 1
        fd.RestoreDirectory = True

        If fd.ShowDialog() = DialogResult.OK Then
            TextBox1.Text = fd.FileName
            Form1_SetExecutableState()
        End If
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        If CheckUnsavedChanges() Then
            Exit Sub
        End If
        If Button2.Enabled = False Then
            Exit Sub
        End If
        Button2.Enabled = False
        Helpers.FlipStatus(TextBox1.Text)
        Form1_SetExecutableState()

    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        For Each item In CheckedListBox1.Items
            Dim checked As Boolean = CheckedListBox1.GetItemChecked(CheckedListBox1.Items.IndexOf(item))
            Dim patch As JSON_Config_Patch = config.FindPatchByName(item.ToString())
            If patch IsNot Nothing Then
                If checked Then
                    patch.apply = "true"
                Else
                    patch.apply = "false"
                End If
            End If
        Next
        Helpers.WriteConfigJSON(My.Computer.FileSystem.GetParentPath(TextBox1.Text), config)
        Button3.Enabled = False
    End Sub

    Private Sub CheckedListBox1_ItemCheck(sender As Object, e As EventArgs) Handles CheckedListBox1.ItemCheck
        Button3.Enabled = True
    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click
        Form1_SetExecutableState()
    End Sub

    Private Sub Form1_FormClosing(sender As Object, e As FormClosingEventArgs) Handles MyBase.FormClosing
        If CheckUnsavedChanges() Then
            e.Cancel = True
        End If
    End Sub
End Class
