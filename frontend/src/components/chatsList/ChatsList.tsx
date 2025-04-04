import cls from "@/utils/cls"
import styles from "./ChatsList.module.css"
import { ChatsItem } from "./chatsItem/ChatsItem"
import { IChat } from "@/types"
import { Button, Modal, Shadow, Input } from "@/components/UI"
import { useRef, useState } from "react"
import useInput from "@/hooks/useInput"
// import ChatService from "@/api/ChatService"

interface ChatsListProps {
    className?: string
    onClick?: (chat: IChat) => void
    createNewChat: (name: string) => void
    data: IChat[] | null
}

const addChatData: IChat = {
    id: 0,
    name: "Add Chat",
    createdAt: "",
    img: "/add_chat.svg"
}

export const ChatsList = ({
    className = "",
    onClick = () => {},
    createNewChat,
    data
}: ChatsListProps): JSX.Element => {
    const [addChat, setAddChat] = useState(false)
    const [value, _] = useInput("")

    const onCreateNewChat = (e: React.FormEvent<HTMLFormElement>) => {
        e.preventDefault()
        if (value.value !== "") {
            createNewChat(value.value)
        }
        setAddChat(false)
    }

    return (
        <>
            { addChat && <>
                <Shadow className={styles.shadow} onClick={() => setAddChat(false)} />
                <Modal exit onExit={() => setAddChat(false)} className={styles.create_chat_modal}>
                    <form action="" onSubmit={(e) => onCreateNewChat(e)}>
                        <Input autoFocus placeholder="Chat name" {...value} />
                        <Button type="submit">Create</Button>
                    </form>
                </Modal>
            </> }
            <ul className={cls(styles.list, className)}>
                <ChatsItem onClick={() => setAddChat((prev: boolean) => !prev)} data={addChatData} index={null} key={0} />
                { data && data.map((item: IChat, index: number): JSX.Element => (
                    <ChatsItem data={item} index={index+1} key={index} onClick={onClick} />
                ))}
            </ul>
        </>
    )
}
