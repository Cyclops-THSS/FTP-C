# -*- mode: python -*-

block_cipher = None


a = Analysis(['client.py'],
             pathex=['/Users/Cyclops-THSS/Documents/2016Fall/Computer_NetWork_1/FTP_C/client'],
             binaries=None,
             datas=None,
             hiddenimports=[],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          name='client',
          debug=False,
          strip=False,
          upx=True,
          console=False )
app = BUNDLE(exe,
             name='client.app',
             icon=None,
             bundle_identifier=None,
             info_plist={
                'NSHighResolutionCapable': 'True'
                })
