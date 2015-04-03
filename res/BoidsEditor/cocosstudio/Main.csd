<GameProjectFile>
  <PropertyGroup Type="Scene" Name="Main" ID="2dce7caa-b916-4fba-9a8e-73781cdf6707" Version="2.2.1.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Scene" FrameEvent="" ctype="SingleNodeObjectData">
        <Position X="0.0000" Y="0.0000" />
        <Scale ScaleX="1.0000" ScaleY="1.0000" />
        <AnchorPoint />
        <CColor A="255" R="255" G="255" B="255" />
        <Size X="1920.0000" Y="1080.0000" />
        <PrePosition X="0.0000" Y="0.0000" />
        <PreSize X="0.0000" Y="0.0000" />
        <Children>
          <NodeObjectData Name="root" ActionTag="626" FrameEvent="" Tag="133" TouchEnable="True" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="960.0000" Y="540.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="600.0000" Y="1000.0000" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="0.3125" Y="0.9259" />
            <Children>
              <NodeObjectData Name="input_mapPath" ActionTag="627" FrameEvent="" Tag="134" LeftMargin="22.0000" RightMargin="178.0000" TopMargin="29.5000" BottomMargin="943.5000" TouchEnable="True" FontSize="14" IsCustomSize="True" LabelText="/Users/chenyanjie/Documents/code/Boids/res/maps" PlaceHolderText="地图文件夹" MaxLengthText="20" ctype="TextFieldObjectData">
                <Position X="222.0000" Y="957.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="51" G="51" B="51" />
                <Size X="400.0000" Y="27.0000" />
                <PrePosition X="0.3700" Y="0.9570" />
                <PreSize X="0.6667" Y="0.0270" />
              </NodeObjectData>
              <NodeObjectData Name="button_load" ActionTag="629" FrameEvent="" Tag="136" TouchEnable="True" FontSize="14" ButtonText="加载" Scale9Enable="True" Scale9Width="46" Scale9Height="36" ctype="ButtonObjectData">
                <Position X="500.0000" Y="960.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="80.0000" Y="40.0000" />
                <PrePosition X="0.8333" Y="0.9600" />
                <PreSize X="0.0417" Y="0.0370" />
                <TextColor A="255" R="0" G="179" B="255" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" />
              </NodeObjectData>
              <NodeObjectData Name="button_edit" ActionTag="630" FrameEvent="" Tag="137" TouchEnable="True" FontSize="14" ButtonText="编辑" Scale9Enable="True" Scale9Width="46" Scale9Height="36" ctype="ButtonObjectData">
                <Position X="150.0000" Y="60.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="80.0000" Y="40.0000" />
                <PrePosition X="0.2500" Y="0.0600" />
                <PreSize X="0.1333" Y="0.0500" />
                <TextColor A="255" R="0" G="179" B="255" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" />
              </NodeObjectData>
              <NodeObjectData Name="button_play" ActionTag="631" FrameEvent="" Tag="138" TouchEnable="True" FontSize="14" ButtonText="试玩" Scale9Enable="True" Scale9Width="46" Scale9Height="36" ctype="ButtonObjectData">
                <Position X="300.0000" Y="60.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="80.0000" Y="40.0000" />
                <PrePosition X="0.5000" Y="0.0600" />
                <PreSize X="0.1333" Y="0.0500" />
                <TextColor A="255" R="0" G="179" B="255" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" />
              </NodeObjectData>
              <NodeObjectData Name="button_exit" ActionTag="633" FrameEvent="" Tag="140" TouchEnable="True" FontSize="14" ButtonText="退出" Scale9Enable="True" Scale9Width="46" Scale9Height="36" ctype="ButtonObjectData">
                <Position X="450.0000" Y="60.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="80.0000" Y="40.0000" />
                <PrePosition X="0.7500" Y="0.0600" />
                <PreSize X="0.1333" Y="0.0400" />
                <TextColor A="255" R="221" G="75" B="57" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="255" G="255" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
        </Children>
      </ObjectData>
    </Content>
    <CameraPosition ValueX="0.0000" ValueY="5.0000" ValueZ="-1.0000" />
    <CameraRotation ValueX="0.0000" ValueY="0.0000" ValueZ="0.0000" />
  </Content>
</GameProjectFile>