import matplotlib.pyplot as plt


# 파일 읽기
with open('t_rate.txt', 'r') as file:
    lines = file.readlines()

# x축 데이터 줄 번호: 3, 5, 7, 9, 11
x1_lines = [2, 4, 6, 8, 10]  # 파이썬의 인덱싱은 0부터 시작하므로 1씩 빼줌
# y축 데이터 줄 번호: 4, 6, 8, 10, 12
y1_lines = [3, 5, 7, 9, 11]  # 파이썬의 인덱싱은 0부터 시작하므로 1씩 빼줌

# x축과 y축 데이터 추출
x1_data = [lines[i].strip() for i in x1_lines]
y1_data = [float(lines[i].strip()) for i in y1_lines]

# 꺾은선 그래프 그리기
plt.figure(figsize=(8, 5))  # 그래프 크기 설정
plt.plot(x1_data, y1_data, marker='o')  # 데이터 플롯, 점으로 표시
plt.title("WCMD Total rate")  # 그래프 제목
plt.xlabel("Time")  # x축 제목
plt.ylabel("Rate")  # y축 제목
plt.grid(False)  # 그리드 표시
plt.xticks(rotation=45)  # x축 라벨 회전
plt.tight_layout()  # 레이아웃 조정

#plt.show()  # 그래프 보여주기
plt.savefig("./figure/wcmd_rate.png")

# x축 데이터 줄 번호: 3, 5, 7, 9, 11
x2_lines = [12, 14, 16, 18, 20]  # 파이썬의 인덱싱은 0부터 시작하므로 1씩 빼줌
# y축 데이터 줄 번호: 4, 6, 8, 10, 12
y2_lines = [13, 15, 17, 19, 21]  # 파이썬의 인덱싱은 0부터 시작하므로 1씩 빼줌

# x축과 y축 데이터 추출
x2_data = [lines[i].strip() for i in x2_lines]
y2_data = [float(lines[i].strip()) for i in y2_lines]

# 꺾은선 그래프 그리기
plt.figure(figsize=(8, 5))  # 그래프 크기 설정
plt.plot(x2_data, y2_data, marker='o')  # 데이터 플롯, 점으로 표시
plt.title("PSMD1 Total rate")  # 그래프 제목
plt.xlabel("Time")  # x축 제목
plt.ylabel("Rate")  # y축 제목
plt.grid(False)  # 그리드 표시
plt.xticks(rotation=45)  # x축 라벨 회전
plt.tight_layout()  # 레이아웃 조정

#plt.show()  # 그래프 보여주기
plt.savefig("./figure/psmd1_rate.png")

# x축 데이터 줄 번호: 3, 5, 7, 9, 11
x3_lines = [22, 24, 26, 28, 30]  # 파이썬의 인덱싱은 0부터 시작하므로 1씩 빼줌
# y축 데이터 줄 번호: 4, 6, 8, 10, 12
y3_lines = [23, 25, 27, 29, 31]  # 파이썬의 인덱싱은 0부터 시작하므로 1씩 빼줌

# x축과 y축 데이터 추출
x3_data = [lines[i].strip() for i in x3_lines]
y3_data = [float(lines[i].strip()) for i in y3_lines]

# 꺾은선 그래프 그리기
plt.figure(figsize=(8, 5))  # 그래프 크기 설정
plt.plot(x3_data, y3_data, marker='o')  # 데이터 플롯, 점으로 표시
plt.title("PSMD2 Total rate")  # 그래프 제목
plt.xlabel("Time")  # x축 제목
plt.ylabel("Rate")  # y축 제목
plt.grid(False)  # 그리드 표시
plt.xticks(rotation=45)  # x축 라벨 회전
plt.tight_layout()  # 레이아웃 조정

#plt.show()  # 그래프 보여주기
plt.savefig("./figure/psmd2_rate.png")


'''

import matplotlib.pyplot as plt

# 파일 읽기
with open('t_rate.txt', 'r') as file:
    lines = file.readlines()

    #wcmd
# 3번째 줄부터 7번째 줄까지의 숫자 추출
x1 = [2,4,6,8,10]
y1 = [3,5,7,9,11]

# x축과 y축 데이터 추출
x1_data = [lines[i].strip() for i in x1]
y1_data = [float(lines[i].strip()) for i in y1]

#꺾은선 그래프 그리기
plt.figure(figsize=(10, 5))  # 그래프 크기 설정
plt.plot(x1_data, y1_data, marker='o')  # 데이터 플롯, 점으로 표시
plt.title("Line Graph of Numbers")  # 그래프 제목
plt.ylabel("Total Rate")  # y축 제목
plt.xlabel("Time")  # x축 제목

# x축 눈금 설정 (0부터 시작해서 데이터 수에 맞게)
#plt.xticks(range(len(y1_data)))  # x축 포인트 설정

plt.grid(False)  # 그리드 표시
#plt.show()  # 그래프 보여주기



#iadc1
# 3번째 줄부터 7번째 줄까지의 숫자 추출
data = [float(line.strip()) for line in lines[7:12]]  # lines[2:7]은 3번째 줄부터 7번째 줄까지

# 꺾은선 그래프 그리기
plt.figure(figsize=(10, 5))  # 그래프 크기 설정
plt.plot(data, marker='o')  # 데이터 플롯, 점으로 표시
plt.title("Line Graph of Numbers")  # 그래프 제목
plt.ylabel("Total Rate")  # y축 제목
plt.xlabel("Time")  # x축 제목

# x축 눈금 설정 (0부터 시작해서 데이터 수에 맞게)
plt.xticks(range(len(data)))  # x축 포인트 설정

plt.grid(False)  # 그리드 표시
#plt.show()  # 그래프 보여주기
plt.savefig("psmd1_rate.png")

# 3번째 줄부터 7번째 줄까지의 숫자 추출
data = [float(line.strip()) for line in lines[12:17]]  # lines[2:7]은 3번째 줄부터 7번째 줄까지

# 꺾은선 그래프 그리기
plt.figure(figsize=(10, 5))  # 그래프 크기 설정
plt.plot(data, marker='o')  # 데이터 플롯, 점으로 표시
plt.title("Line Graph of Numbers")  # 그래프 제목
plt.ylabel("Total Rate")  # y축 제목
plt.xlabel("Time")  # x축 제목

# x축 눈금 설정 (0부터 시작해서 데이터 수에 맞게)
plt.xticks(range(len(data)))  # x축 포인트 설정

plt.grid(False)  # 그리드 표시
#plt.show()  # 그래프 보여주기
plt.savefig("psmd2_rate.png")
'''
