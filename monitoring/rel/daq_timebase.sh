#!/bin/bash

# 디렉토리 설정
LOG_DIR="/data/amore2test/LOG"
OUTPUT_FILE="time.txt"

# FADCDAQ로 시작하는 파일들 중 가장 최신 파일 찾기 (제외할 파일)
exclude_file=$(ls -1t "$LOG_DIR"/FADCDAQ_* | head -n 1)

# 변수 초기화
total_sum=0

# FADCDAQ로 시작하는 파일들 중 최신 파일을 제외한 나머지 파일들에 대해 작업 수행
while IFS= read -r file; do
    # 파일명에서 숫자 부분 추출 (FADCDAQ_ 제외)
    file_number=$(basename "$file" | sed 's/FADCDAQ_//' | sed 's/\.log//')

    # Live time : xx [s] 패턴을 찾아서 xx 부분 추출
    total_time=$(grep -oP 'Live time : \K[0-9]+' "$file")

    # 결과 출력
    echo "${file_number}	${total_time}" >> "$OUTPUT_FILE"

    # 총합 계산
    total_sum=$((total_sum + total_time))
done < <(ls -1 "$LOG_DIR"/FADCDAQ_* | grep -v "$(basename "$exclude_file")")

# 총합 결과를 파일에 추가 기록
echo "$total_sum" >> "$OUTPUT_FILE"
