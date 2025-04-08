import cls from "@/utils/cls"
import styles from "./ChatsList.module.css"
import { ChatsItem } from "./chatsItem/ChatsItem"
import { IChat } from "@/types"
import { Button, Modal, Shadow, Input } from "@/components/UI"
import { useState } from "react"
import useInput from "@/hooks/useInput"
// import ChatService from "@/api/ChatService"

interface ChatsListProps {
    className?: string
    onClick?: (chat: IChat) => void
    createNewChat: (name: string) => void
    removeChat: (chatId: number) => void
    data: IChat[]
}

export const ChatsList = ({
    className = "",
    onClick = () => {},
    createNewChat,
    removeChat,
    data = []
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
                { data.slice().reverse().map((item: IChat, index: number): JSX.Element => (
                    <ChatsItem data={item} index={index+1} key={index} onClick={onClick} onClickRemove={removeChat} />
                ))}
            </ul>
        </>
    )
}
