import { useChatStore } from "@/state/chat"
import { IChat } from "@/types"
import cls from "@/utils/cls"
import styles from "./ChatsList.module.css"
import { ChatsItem } from "./chatsItem/ChatsItem"

interface ChatsListProps {
    className?: string
    onClick?: (chat: IChat) => void
    createNewChat?: (name: string) => void
    removeChat: (chatId: number) => void
    data: IChat[]
}

export const ChatsList = ({
    className = "",
    onClick = () => {},
    removeChat,
    data = []
}: ChatsListProps): JSX.Element => {
    const currentChat = useChatStore(state => state.data)
    const compareTimes = (time1: string | undefined, time2: string | undefined) => {
        if (!time1 || !time2) return 0;
        time1 =time1.split(" ").join("T");
        time2 =time2.split(" ").join("T");
        const date1 = new Date(time1);
        const date2 = new Date(time2);

        if (date1 > date2) {
            return -1;
        } else if (date1 < date2) {
            return 1;
        } else {
            return 0;
        }
    }

    return (
        <>
            <ul className={cls(styles.list, className)}>
                { data
                    .slice()
                    .sort((a: IChat, b: IChat) => compareTimes(a.lastMessage?.createdAt, b.lastMessage?.createdAt))
                    .map((item: IChat, index: number): JSX.Element => (
                        <ChatsItem data={item} index={index+1} key={index} onClick={onClick} onClickRemove={removeChat} className={currentChat?.id === item.id ? styles.active : ""} />
                    ))}
            </ul>
        </>
    )
}
