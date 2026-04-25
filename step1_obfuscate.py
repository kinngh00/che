import os
import re
import random
import string

# 1. 설정: 난독화 대상 경로
TARGET_DIR = r'C:\Users\KinngH\Desktop\cheat-engine-7.5'
EXTENSIONS = ('.c', '.h', '.pas')

# 2. 난독화용 무작위 문자열 생성 (6자리)
def rand_name():
  return ''.join(random.choices(string.ascii_lowercase + string.digits, k=6))

# 3. 핵심 문자열 -> 16진수 변환
def to_hex_array(match):
  original_str = match.group(1)
  hex_data = ', '.join([hex(ord(c)) for c in original_str])
  return f"/* hidden string */ {{ {hex_data}, 0x00 }}"

def obfuscate_file(filepath):
  with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
    content = f.read()

  # A. 시그니처 문자열 은닉 (Cheat Engine, Lua 등)
  # 소스 내의 "문자열" 패턴을 찾아 주석처리하거나 변조하는 기초 로직
  targets = ['Cheat Engine', 'CheatEngine', 'DBK64', 'Lua 5.3', 'Copyright']
  for t in targets:
    content = content.replace(f'"{t}"', f'"{rand_name()}"') # 단순 치환

  # B. 정크 코드 삽입 (함수 시작 부분 { 뒤에 삽입)
  junk_code = f"\n    if ((0x{random.randint(1000, 9999):x} ^ 0xbad) == 0) {{ __asm {{ nop }} }}\n"
  content = re.sub(r'{\n', r'{\n' + junk_code, content, count=10) # 파일당 최대 10곳

  # C. static 함수명 난독화 (기초적인 정규식 예시)
  # 주의: 모든 함수를 바꾸면 링크 에러가 나므로 static 위주로 권장
  static_funcs = re.findall(r'static \w+ (\w+)\s*\(', content)
  for func in set(static_funcs):
    if func not in ['main', 'WinMain']:
      content = content.replace(func, "ce_" + rand_name())

  with open(filepath, 'w', encoding='utf-8') as f:
    f.write(content)
  print(f"[완료] {filepath}")

# 실행부
if __name__ == "__main__":
  for root, dirs, files in os.walk(TARGET_DIR):
    for file in files:
      if file.endswith(EXTENSIONS):
        full_path = os.path.join(root, file)
        try:
          obfuscate_file(full_path)
        except Exception as e:
          print(f"[실패] {file}: {e}")
  print("\n--- 모든 파일 난독화 완료. 이제 컴파일을 시도해 보세요. ---")